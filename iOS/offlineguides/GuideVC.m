#import "GuideVC.h"

@interface GuideVC ()

@property (nonatomic, strong) UIWebView * webView;

@end

@implementation GuideVC

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
  self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
  if (self)
  {
    _webView = [[UIWebView alloc] init];
    [self.webView scalesPageToFit];
    [self.webView setFrame:self.view.frame];
    self.view  = self.webView;
    [self.view setBackgroundColor:[UIColor redColor]];
  }
  return self;
}

- (void)viewDidLoad
{
  [super viewDidLoad];
  NSString * pathtoHtml = [[NSBundle mainBundle] pathForResource:@"simple_html" ofType:@"html"];
  NSURL * url = [NSURL fileURLWithPath: pathtoHtml isDirectory:NO];
  [self.webView loadRequest:[NSURLRequest requestWithURL: url]];
}

- (void)viewWillAppear:(BOOL)animated
{
  [super viewWillAppear:animated];
  [self.navigationController setNavigationBarHidden:NO animated:NO];
}

@end
