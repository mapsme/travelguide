#import "IPadSplitVC.h"
#import "GuideVC.h"

@interface IPadSplitVC ()

@end

@implementation IPadSplitVC

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self)
    {
      self.delegate = self;
      ArticleVC * vc = [[ArticleVC alloc] initWithStyle:UITableViewStylePlain];
      vc.delegate = self;
      UINavigationController * v = [[UINavigationController alloc] initWithRootViewController:[[GuideVC alloc] init]];
      v.navigationBarHidden = NO;
      self.viewControllers = @[vc, v];
    }
    return self;
}

- (void)viewDidLoad
{
  [super viewDidLoad];
}

-(void)viewWillAppear:(BOOL)animated
{
  [super viewWillAppear:animated];
  ArticleVC * vc = (ArticleVC *)[self.viewControllers objectAtIndex:0];
  UINavigationController * navVC = (UINavigationController*)[self.viewControllers objectAtIndex:1];
  GuideVC * g = (GuideVC *)navVC.visibleViewController;
  [g loadPage:[vc getDefaultArticle]];
}

-(void)selectHtmlPageUrl:(NSString *)url
{
  UINavigationController * navVC = (UINavigationController*)[self.viewControllers objectAtIndex:1];
  GuideVC * g = (GuideVC *)navVC.visibleViewController;
  g.navigationItem.leftBarButtonItem = nil;
  [g loadPage:url];
  [self.pop dismissPopoverAnimated:YES];
}

#pragma mark - Split view

- (void)splitViewController:(UISplitViewController *)splitController willHideViewController:(UIViewController *)viewController withBarButtonItem:(UIBarButtonItem *)barButtonItem forPopoverController:(UIPopoverController *)popoverController
{
  UIImage * backButton = [UIImage imageNamed:@"ic_articleselection"];

  UIButton * button = [UIButton buttonWithType:UIButtonTypeCustom];
  [button setImage:backButton forState:UIControlStateNormal];
  button.frame = CGRectMake(0, 0, backButton.size.width, backButton.size.height);
  [button addTarget:barButtonItem.target action:barButtonItem.action forControlEvents:UIControlEventTouchUpInside];
  barButtonItem = [[UIBarButtonItem alloc] initWithCustomView:button];

  UINavigationController * navVC = (UINavigationController*)[self.viewControllers objectAtIndex:1];
  [navVC.topViewController.navigationItem setRightBarButtonItem:barButtonItem animated:YES];
  self.pop = popoverController;
}

- (void)splitViewController:(UISplitViewController *)splitController willShowViewController:(UIViewController *)viewController invalidatingBarButtonItem:(UIBarButtonItem *)barButtonItem
{
  UINavigationController * navVC = (UINavigationController*)[self.viewControllers objectAtIndex:1];
  [navVC.topViewController.navigationItem setRightBarButtonItem:nil animated:YES];
  self.pop = nil;
}

@end
