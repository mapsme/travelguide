#import <UIKit/UIKit.h>

@interface GuideVC : UIViewController <UIWebViewDelegate, UIGestureRecognizerDelegate>

@property (nonatomic, assign) int numberOfPages;

-(void)loadPage:(NSString *)pageUrl;
-(void)clearPreviosViews;
-(NSString *)getCurrentUrl;

@end
