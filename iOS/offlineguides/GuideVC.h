#import <UIKit/UIKit.h>

@interface GuideVC : UIViewController <UIWebViewDelegate, UIGestureRecognizerDelegate>

-(void)loadPage:(NSString *)pageUrl;
@property (nonatomic, strong) NSMutableArray * webPages;

-(void)clearPreviosViews;

@end
