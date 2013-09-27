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
  {
    self.navigationItem.rightBarButtonItem =  [self getCustomButtonWithImage:@"ic_articleselection"];
    self.navigationItem.leftBarButtonItem =  [self getCustomButtonWithImage:@"ic_back"];
  }
  self.navigationController.navigationBar.titleTextAttributes =
  @{
    UITextAttributeTextColor       : [UIColor colorWithRed:253.f/255.f green:241.f/255.f blue:43.f/255.f alpha:1.f],
    UITextAttributeTextShadowColor : [UIColor clearColor]
  };
  self.navigationItem.title = @"UK GuideWithMe";
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request
 navigationType:(UIWebViewNavigationType)navigationType
{
  // MWM API urls will be opened in openMwmUrl method
  if ([self openMwmUrl:request.URL.absoluteString])
    return NO;

  NSString * str = [self normalizeUrl:[[request URL] absoluteString]];
  if ([self isWebPage:str] && [[UIApplication sharedApplication] canOpenURL:[request URL]])
  {
    [[UIApplication sharedApplication] openURL:[request URL]];
    return NO;
  }
  [self updateArticleView:str];
  if ([self isImage:str])
    self.webView.scalesPageToFit = YES;
  else
    self.webView.scalesPageToFit = NO;
  return YES;
}

- (void)webViewDidFinishLoad:(UIWebView *)webView
{
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
  {
    if ([self.webView canGoBack])
      self.navigationItem.leftBarButtonItem = [self getCustomButtonWithImage:@"ic_back"];
    else
      self.navigationItem.leftBarButtonItem = nil;
  }
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
 if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)
 {
   if ([self.webView canGoBack])
     [self.webView goBack];
   else
     [self.navigationController popViewControllerAnimated:YES];
   return;
 }
 else
 {
   [self.webView goBack];
   if (![self.webView canGoBack])
     self.navigationItem.leftBarButtonItem = nil;
 }
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
    if (NSNotFound != [pageUrl rangeOfString:arr[i]].location)
      return YES;

  return NO;
}

-(BOOL)isWebPage:(NSString *)str
{
  static NSString * const arr [] =
  {
    @"www",
    @"http",
  };

  for (size_t i = 0; i < ArraySize(arr); ++i)
    if (0 == [str rangeOfString:arr[i]].location)
      return YES;

  return NO;
}

-(BOOL)openMwmUrl:(NSString *)str
{
  // Looking for mapswithme://... urls
  if (0 != [str rangeOfString:@"mapswithme" options:NSCaseInsensitiveSearch].location)
    return NO;

  NSString * newUrl = [str stringByAppendingFormat:@"&backurl=%@&appname=%@",
                       [MWMApi detectBackUrlScheme],
                       [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleDisplayName"]];
  if ([MWMApi isApiSupported])
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:newUrl]];
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
    {
      NSString * htmlId = [url substringToIndex:r.location];
      NSRange z = [htmlId rangeOfString:@"/" options:NSBackwardsSearch];
      if (r.length)
        [[self getArticleController] updateView:[htmlId substringFromIndex:z.location + 1]];
      else
        [[self getArticleController] updateView:[htmlId substringToIndex:z.location]];
    }
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

-(NSString *)getCurrentUrl
{
  return [self.webView.request.URL absoluteString];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
  return YES;
}

@end
