#import "GuideVC.h"
#import "ArticleVC.h"
#import "MapsWithMeAPI.h"
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
    UIPinchGestureRecognizer * pinch = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(onPinch:)];
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
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request
 navigationType:(UIWebViewNavigationType)navigationType
{
  NSString * str = [self normalizeUrl:[[request URL] absoluteString]];
  [self.webPages addObject:str];
  if ([self.webPages count]  > 1 && UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    self.navigationItem.leftBarButtonItem =  [self getCustomButtonWithImage:@"ic_back"];
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)
    self.navigationItem.leftBarButtonItem =  [self getCustomButtonWithImage:@"ic_back"];
  return YES;
}

- (unsigned int)textSizeAdjustment
{
  if (m_webViewScale == 0.0)
    m_webViewScale = 1.0;
  return static_cast<unsigned int>(100 * m_webViewScale);
}

-(void)onPinch:(UIPinchGestureRecognizer *)sender
{
//  if (m_webViewScale == 0.0)
//    m_webViewScale = 1.0;
//  if (m_webViewScaleOnStart == 0.0 || sender.state == UIGestureRecognizerStateBegan)
//    m_webViewScaleOnStart = m_webViewScale;
//
//  m_webViewScale = min(4.0f, max(0.25f, m_webViewScaleOnStart * sender.scale));
//
//  [self.webView stringByEvaluatingJavaScriptFromString:
//   [NSString stringWithFormat:
//    @"document.getElementsByTagName('body')[0]"
//    ".style.webkitTextSizeAdjust= '%d%%'", [self textSizeAdjustment]]];

  // @todo kill keyboard after search UINOTIFICATION
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
  {
    NSRange r = [[self.webPages lastObject] rangeOfString:@"." options:NSBackwardsSearch];
    if (r.length)
    {
      NSString * pathToPage = [[NSBundle mainBundle] pathForResource:[[self.webPages lastObject] substringToIndex:r.location]  ofType:@"html" inDirectory:DATAFOLDER];
      NSURL * url = [NSURL fileURLWithPath:pathToPage isDirectory:NO];
      [self.webView loadRequest:[NSURLRequest requestWithURL: url]];
    }
    else
      [self.webView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:[self.webPages lastObject]]]];
  }
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
  ArticleVC * vc = [[ArticleVC alloc] init];
  vc.loadedWebPages = [NSMutableArray arrayWithArray:self.webPages];
  [self.navigationController pushViewController:vc animated:YES];
}
@end
