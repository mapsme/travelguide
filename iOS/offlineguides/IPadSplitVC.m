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
      ArticleVC * articleVC = [[ArticleVC alloc] initWithStyle:UITableViewStylePlain];
      articleVC.delegate = self;
      UINavigationController * navVC = [[UINavigationController alloc] initWithRootViewController:articleVC];
      UINavigationController * v = [[UINavigationController alloc] initWithRootViewController:[[GuideVC alloc] init]];
      v.navigationBarHidden = NO;
      self.viewControllers = @[navVC, v];
      [[self getGuideVC] loadPage:[[self getArticleVC] getDefaultArticle]];
    }
    return self;
}

-(void)viewWillAppear:(BOOL)animated
{
  [super viewWillAppear:animated];
}

-(void)selectHtmlPageUrl:(NSString *)url
{
  GuideVC * g = [[GuideVC alloc] init];
  [g loadPage:url];
  [self replaceGuide:g];
  [[self getArticleVC] killKeyboard];
  [self.pop dismissPopoverAnimated:YES];
}

#pragma mark - Split view

- (void)splitViewController:(UISplitViewController *)splitController willHideViewController:(UIViewController *)viewController withBarButtonItem:(UIBarButtonItem *)barButtonItem forPopoverController:(UIPopoverController *)popoverController
{
  UIImage * guideButtonImage = [UIImage imageNamed:@"ic_articleselection"];
  UIButton * button = [UIButton buttonWithType:UIButtonTypeCustom];
  [button setImage:guideButtonImage forState:UIControlStateNormal];
  button.frame = CGRectMake(0, 0, guideButtonImage.size.width, guideButtonImage.size.height);
  [button addTarget:self action:@selector(showPopover) forControlEvents:UIControlEventTouchUpInside];
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

-(ArticleVC *)getArticleVC
{
  UINavigationController * articleNavVC = [self.viewControllers objectAtIndex:0];
  return (ArticleVC *)articleNavVC.visibleViewController;
}

-(GuideVC *)getGuideVC
{
  UINavigationController * GuideNavVC = (UINavigationController*)[self.viewControllers objectAtIndex:1];
  return (GuideVC *)GuideNavVC.visibleViewController;
}

-(void)replaceGuide:(GuideVC *)guide
{
  UINavigationController * GuideNavVC = (UINavigationController*)[self.viewControllers objectAtIndex:1];

  guide.navigationItem.leftBarButtonItem = nil;
  guide.navigationItem.rightBarButtonItem = GuideNavVC.topViewController.navigationItem.rightBarButtonItem;
  GuideNavVC.topViewController.navigationItem.rightBarButtonItem = nil;
  GuideNavVC.viewControllers = @[guide];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
  return YES;
}

-(void)showPopover
{
  UINavigationController * GuideNavVC = (UINavigationController*)[self.viewControllers objectAtIndex:1];
  [self.pop presentPopoverFromBarButtonItem:GuideNavVC.topViewController.navigationItem.rightBarButtonItem permittedArrowDirections:UIPopoverArrowDirectionUp animated:YES];
}

@end
