#import <UIKit/UIKit.h>
#import "ArticleVC.h"

@interface IPadSplitVC : UISplitViewController <UISplitViewControllerDelegate, ArticleDelegate>
@property (nonatomic, strong) UIPopoverController * pop;
@end
