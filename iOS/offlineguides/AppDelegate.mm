#import "AppDelegate.h"
#import "ArticleVC.h"

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
  self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
  ArticleVC * table = [[ArticleVC alloc] initWithStyle:UITableViewStylePlain];
  self.window.rootViewController = [[UINavigationController alloc] initWithRootViewController:table];
  [self.window makeKeyAndVisible];
  return YES;
}

@end
