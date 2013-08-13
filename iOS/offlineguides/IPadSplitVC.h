#import <UIKit/UIKit.h>
#import "ArticleVC.h"

@class GuideVC;

@interface IPadSplitVC : UISplitViewController <UISplitViewControllerDelegate, ArticleDelegate>
@property (nonatomic, strong) UIPopoverController * pop;

-(void)replaceGuide:(GuideVC *)guide;
@end
