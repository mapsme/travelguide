#import "GuideVC.h"
#import "../../std/algorithm.hpp"

#define DATAFOLDER @"/data/"

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
    UIPinchGestureRecognizer * pinch = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(onPinch:)];
    pinch.delegate = self;
    [self.webView addGestureRecognizer:pinch];
    self.view  = self.webView;
    m_webViewScale = 1.0;
    m_webViewScaleOnStart = 0.0;
  }
  return self;
}


- (void)viewDidLoad
{
  [super viewDidLoad];
}

-(void)loadPage:(NSString *)pageUrl
{
  NSRange r = [pageUrl rangeOfString:@"."];
  NSString * pageName = [pageUrl substringToIndex:r.location];
  NSString * pageType = [pageUrl substringFromIndex:r.location + 1];
  NSString * pathtoPage = [[NSBundle mainBundle] pathForResource:pageName ofType:pageType inDirectory:DATAFOLDER];
  
  NSURL * url = [NSURL fileURLWithPath: pathtoPage isDirectory:NO];
  [self.webView loadRequest:[NSURLRequest requestWithURL: url]];
}

- (void)viewWillAppear:(BOOL)animated
{
  [super viewWillAppear:animated];
  [self.navigationController setNavigationBarHidden:NO animated:NO];
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request
 navigationType:(UIWebViewNavigationType)navigationType
{
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
  if (m_webViewScale == 0.0)
    m_webViewScale = 1.0;
  if (m_webViewScaleOnStart == 0.0 || sender.state == UIGestureRecognizerStateBegan)
    m_webViewScaleOnStart = m_webViewScale;

  m_webViewScale = min(4.0f, max(0.25f, m_webViewScaleOnStart * sender.scale));

  [self.webView stringByEvaluatingJavaScriptFromString:
   [NSString stringWithFormat:
    @"document.getElementsByTagName('body')[0]"
    ".style.webkitTextSizeAdjust= '%d%%'", [self textSizeAdjustment]]];
}

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer
shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognize
{
  return YES;
}

@end
