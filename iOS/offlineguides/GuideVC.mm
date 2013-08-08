#import "GuideVC.h"
#import "ArticleVC.h"
#import "MapsWithMeAPI.h"
#import "AppDelegate.h"
#import "../../std/algorithm.hpp"

#define DATAFOLDER @"/data/"

@interface GuideVC ()
{
  float m_webViewScale;
  float m_webViewScaleOnStart;
}

@property (nonatomic, strong) UIWebView * webView;
@property (nonatomic, strong) NSString * m_guide;

@end

@implementation GuideVC

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
  self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
  if (self)
  {
    _webView = [[UIWebView alloc] init];
    [self.webView setFrame:self.view.frame];
    UIPanGestureRecognizer * pinch = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(onPinch:)];
    pinch.delegate = self;
    [self.webView addGestureRecognizer:pinch];
    self.webView.delegate = self;
    self.view  = self.webView;
    m_webViewScale = 1.0;
    m_webViewScaleOnStart = 0.0;
    _webPages = [[NSMutableArray alloc] initWithObjects:nil];
  }
  return self;
}

- (void)viewDidLoad
{
  [super viewDidLoad];
}

-(void)loadPage:(NSString *)pageUrl
{
  self.m_guide = [pageUrl copy];
  NSRange r = [pageUrl rangeOfString:@"." options:NSBackwardsSearch];
  NSString * pathToPage;
  if (r.length == 0)
    pathToPage = [[NSBundle mainBundle] pathForResource:pageUrl ofType:@"html" inDirectory:DATAFOLDER];
  else
    pathToPage = [[NSBundle mainBundle] pathForResource:[pageUrl substringToIndex:r.location] ofType:@"html" inDirectory:DATAFOLDER];
  NSURL * url = [NSURL fileURLWithPath:pathToPage isDirectory:NO];
  [self.webView loadRequest:[NSURLRequest requestWithURL: url]];
}

- (void)viewWillAppear:(BOOL)animated
{
  [super viewWillAppear:animated];
  [self.navigationController setNavigationBarHidden:NO animated:NO];
  [self.navigationController.navigationBar setBackgroundImage:[UIImage imageNamed:@"bg_header.png"] forBarMetrics:UIBarMetricsDefault];
  [self.navigationController.navigationBar setBackgroundImage:[UIImage imageNamed:@"bg_header.png"] forBarMetrics:UIBarMetricsLandscapePhone];

  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)
    self.navigationItem.rightBarButtonItem =  [self getCustomButtonWithImage:@"ic_articleselection"];
  if ([self.webPages count])
    self.navigationItem.leftBarButtonItem =  [self getCustomButtonWithImage:@"ic_back"];
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)
    self.navigationItem.leftBarButtonItem =  [self getCustomButtonWithImage:@"ic_back"];
  self.navigationController.navigationBar.titleTextAttributes = [NSDictionary dictionaryWithObject:[UIColor colorWithRed:253.f/255.f
                                                                                                                   green:241.f/255.f
                                                                                                                    blue:43.f/255.f
                                                                                                                   alpha:1.f] forKey:UITextAttributeTextColor];
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request
 navigationType:(UIWebViewNavigationType)navigationType
{
  NSString * str = [self normalizeUrl:[[request URL] absoluteString]];
  ArticleVC * v;
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
  {
    UISplitViewController * splitControl =  (UISplitViewController *)[[UIApplication sharedApplication] delegate].window.rootViewController;
    v = [splitControl.viewControllers objectAtIndex:0];
  }
  else
    v = [self.navigationController.viewControllers objectAtIndex:0];
  NSRange r = [str rangeOfString:@"." options:NSBackwardsSearch];
  self.navigationItem.title = [v getArticleName:[str substringToIndex:r.location]];
  [self.webPages addObject:str];
  if ([self isImage:str])
    self.webView.scalesPageToFit = YES;
  else
    self.webView.scalesPageToFit = NO;
  if ([self.webPages count]  > 1 && UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    self.navigationItem.leftBarButtonItem =  [self getCustomButtonWithImage:@"ic_back"];
  return YES;
}

- (unsigned int)textSizeAdjustment
{
  if (m_webViewScale == 0.0)
    m_webViewScale = 1.0;
  return static_cast<unsigned int>(100 * m_webViewScale);
}

-(void)onPinch:(UIPanGestureRecognizer *)sender
{
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
  {
    UISplitViewController * splitControl =  (UISplitViewController *)[[UIApplication sharedApplication] delegate].window.rootViewController;
    [[splitControl.viewControllers objectAtIndex:0] killKeyboard];
  }
}

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer
shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognize
{
  return YES;
}

-(void)showSearch
{
  [self.navigationController popToRootViewControllerAnimated:YES];
}

-(void)back
{
  [self.webPages removeLastObject];
  if ([self.webPages count] == 0)
  {
    [self.navigationController popToRootViewControllerAnimated:YES];
    return;
  }
  else
    [self.webView goBack];
  if ([self.webPages count]  == 1 && UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    self.navigationItem.leftBarButtonItem = nil;
  [self.webPages removeLastObject];

}

-(UIBarButtonItem *)getCustomButtonWithImage:(NSString *)name
{
  UIImage * backButton = [UIImage imageNamed:name];

  UIButton * button = [UIButton buttonWithType:UIButtonTypeCustom];
  [button setImage:backButton forState:UIControlStateNormal];
  button.frame = CGRectMake(0, 0, backButton.size.width, backButton.size.height);

  if ([name isEqualToString:@"ic_back"])
    [button addTarget:self action:@selector(back) forControlEvents:UIControlEventTouchUpInside];
  else
    [button addTarget:self action:@selector(goToMainMenu) forControlEvents:UIControlEventTouchUpInside];

  return [[UIBarButtonItem alloc] initWithCustomView:button];
}

-(NSString *)normalizeUrl:(NSString *)url
{
  if ([url rangeOfString:@"offlineguides.app//data/"].location != NSNotFound)
  {
    NSRange r = [url rangeOfString:@"/" options:NSBackwardsSearch];
    NSString * s = [url substringFromIndex:r.location + 1];
    return s;
  }
  return url;
}

-(void)clearPreviosViews
{
  self.navigationController.viewControllers = @[[self.navigationController.viewControllers objectAtIndex:0], [self.navigationController.viewControllers lastObject]];
}

-(void)goToMainMenu
{
  [self.navigationController popToRootViewControllerAnimated:YES];
}

-(BOOL)isImage:(NSString *)pageUrl
{
  return (([pageUrl rangeOfString:@".svg"].location != NSNotFound) || ([pageUrl rangeOfString:@".png"].location != NSNotFound) || ([pageUrl rangeOfString:@".jpg"].location != NSNotFound));
}
@end
