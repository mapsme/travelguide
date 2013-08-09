#import "AppDelegate.h"
#import "ArticleVC.h"
#import "IPadSplitVC.h"
#import "MapsWithMeAPI.h"

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

- (BOOL)application:(UIApplication *)application
            openURL:(NSURL *)url
  sourceApplication:(NSString *)sourceApplication
         annotation:(id)annotation
{
  if ([MWMApi isMapsWithMeUrl:url])
  {
    MWMPin * pin = [MWMApi pinFromUrl:url];
    if (pin)
    {
      // @TODO Show corresponding article
      NSLog(@"@TODO Show article %@", pin.idOrUrl);
    }
    return YES;
  }
  return NO;
}

@end
