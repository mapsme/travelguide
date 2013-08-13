#import "GuideVC.h"
#import "ArticleVC.h"
#import "MapsWithMeAPI.h"
#import "AppDelegate.h"
#import "../../std/algorithm.hpp"
#import "../../std/array.hpp"

#define DATAFOLDER @"/data/"
#define INDECATORBORDER 100

@interface GuideVC ()
{
  float m_webViewScale;
  float m_webViewScaleOnStart;
}

@property (nonatomic, strong) UIWebView * webView;
@property (nonatomic, strong) UIActivityIndicatorView * indicator;

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
    self.numberOfPages = 0;
    _indicator = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleGray];
    [self.webView addSubview:self.indicator];
  }
  return self;
}

-(void)loadPage:(NSString *)pageUrl
{
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
  [self.navigationController.navigationBar setBackgroundImage:[UIImage imageNamed:@"bg_header"] forBarMetrics:UIBarMetricsDefault];
  [self.navigationController.navigationBar setBackgroundImage:[UIImage imageNamed:@"bg_header"] forBarMetrics:UIBarMetricsLandscapePhone];

  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)
    self.navigationItem.rightBarButtonItem =  [self getCustomButtonWithImage:@"ic_articleselection"];
  if (self.numberOfPages)
    self.navigationItem.leftBarButtonItem =  [self getCustomButtonWithImage:@"ic_back"];
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)
    self.navigationItem.leftBarButtonItem =  [self getCustomButtonWithImage:@"ic_back"];
  self.navigationController.navigationBar.titleTextAttributes = @{UITextAttributeTextColor : [UIColor colorWithRed:253.f/255.f
                                                                                                             green:241.f/255.f
                                                                                                              blue:43.f/255.f
                                                                                                             alpha:1.f],
                                                                  UITextAttributeTextShadowColor: [UIColor clearColor]};
  self.navigationItem.title = @"UK GuideWithMe";
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request
 navigationType:(UIWebViewNavigationType)navigationType
{
  NSString * str = [self normalizeUrl:[[request URL] absoluteString]];
  if ([self openMwmUrl:str])
    return NO;
  if ([self isWebPage:str] && [[UIApplication sharedApplication] canOpenURL:[request URL]])
  {
    [[UIApplication sharedApplication] openURL:[request URL]];
    return NO;
  }
  [self performSelector:@selector(addActivityIndicator) withObject:nil afterDelay:0.5];
  [self updateArticleView:str];
  ++self.numberOfPages;
  if ([self isImage:str])
    self.webView.scalesPageToFit = YES;
  else
    self.webView.scalesPageToFit = NO;
  if (self.numberOfPages > 1 && UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    self.navigationItem.leftBarButtonItem =  [self getCustomButtonWithImage:@"ic_back"];
  return YES;
}

- (void)webViewDidFinishLoad:(UIWebView *)webView
{
  [self stopAndHideIndicator];
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
  [self stopAndHideIndicator];
}

-(void)onPinch:(UIPanGestureRecognizer *)sender
{
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
  {
    UISplitViewController * splitControl =  (UISplitViewController *)[[UIApplication sharedApplication] delegate].window.rootViewController;
    ArticleVC * v = (ArticleVC *)((UINavigationController *)[splitControl.viewControllers objectAtIndex:0]).visibleViewController;
    [v killKeyboard];
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
  --self.numberOfPages;
  if (self.numberOfPages <= 0)
  {
    self.numberOfPages = 0;
    [self.navigationController popToRootViewControllerAnimated:YES];
    return;
  }
  else
  {
    [self.webView goBack];
    [self updateArticleView:[self normalizeUrl:[[self.webView.request URL] absoluteString]]];
  }
  if (self.numberOfPages  <= 1 && UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    self.navigationItem.leftBarButtonItem = nil;
  --self.numberOfPages;
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
  static NSString  * const arr [] =
  {
    @".svg",
    @".png",
    @".jpg"
  };
  for (size_t i = 0; i < ArraySize(arr); ++i)
  {
    NSRange r = [pageUrl rangeOfString:arr[i]];
    if (r.location != NSNotFound)
      return YES;
  }
  return NO;
}

-(BOOL)isWebPage:(NSString *)str
{
  static NSString  * const arr [] =
  {
    @"www",
    @"http",
  };
  for (size_t i = 0; i < ArraySize(arr); ++i)
  {
    NSRange r = [str rangeOfString:arr[i]];
    if (r.location == 0)
      return YES;
  }
  return NO;
}

-(BOOL)openMwmUrl:(NSString *)str
{
  NSRange r = [str rangeOfString:@"mapswithme" options:NSBackwardsSearch];
  if (r.location == NSNotFound)
    return NO;
  if ([MWMApi isApiSupported])
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:str]];
  else
    [MWMApi showMapsWithMeIsNotInstalledDialog];
  return YES;
}

-(void)updateArticleView:(NSString *)url
{
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
  {
    NSRange r = [url rangeOfString:@"." options:NSBackwardsSearch];
    if (r.location + 4 >= [url length])
      return;
    if (r.length && [[url substringWithRange:NSRange{r.location + 1, 4}] isEqualToString:@"html"])
      [[self getArticleController] updateView:[url substringToIndex:r.location]];
  }
}

-(ArticleVC *)getArticleController
{
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
  {
    UISplitViewController * splitControl =  (UISplitViewController *)[[UIApplication sharedApplication] delegate].window.rootViewController;
    UINavigationController * navVC = [splitControl.viewControllers objectAtIndex:0];
    return (ArticleVC *)navVC.visibleViewController;
  }
  else
    return [self.navigationController.viewControllers objectAtIndex:0];
}

-(void)addActivityIndicator
{
  CGRect wvf = self.webView.frame;
  [self.indicator setFrame:CGRectMake((wvf.size.width - INDECATORBORDER) / 2, (wvf.size.height - INDECATORBORDER) / 2, INDECATORBORDER, INDECATORBORDER)];
  [self.indicator startAnimating];
}

-(void)stopAndHideIndicator
{
  [NSObject cancelPreviousPerformRequestsWithTarget:self];
  if ([self.indicator isAnimating])
  {
    [self.indicator stopAnimating];
    self.indicator.frame = CGRectZero;
  }
}

-(NSString *)getCurrentUrl
{
  return [self.webView.request.URL absoluteString];
}
@end
