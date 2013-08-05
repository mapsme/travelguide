#import <UIKit/UIKit.h>

@interface GuideVC : UIViewController <UIWebViewDelegate, UIGestureRecognizerDelegate>

-(void)loadPage:(NSString *)pageUrl;

@end
