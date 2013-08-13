#import <UIKit/UIKit.h>

@interface GuideVC : UIViewController <UIWebViewDelegate, UIGestureRecognizerDelegate>

-(void)loadPage:(NSString *)pageUrl;
-(void)clearPreviosViews;
-(NSString *)getCurrentUrl;

@end
