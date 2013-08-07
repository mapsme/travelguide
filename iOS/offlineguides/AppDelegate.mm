#import "AppDelegate.h"
#import "ArticleVC.h"
#import "IPadSplitVC.h"

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
  self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
  if (UI_USER_INTERFACE_IDIOM() != UIUserInterfaceIdiomPad)
    self.window.rootViewController = [[UINavigationController alloc] initWithRootViewController:[[ArticleVC alloc] initWithStyle:UITableViewStylePlain]];
  else
    self.window.rootViewController = [[IPadSplitVC alloc] init];
  [self.window makeKeyAndVisible];
  return YES;
}

@end
