#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h>

@protocol ArticleDelegate <NSObject>
-(void)selectHtmlPageUrl:(NSString *)url;
@end

@interface ArticleVC : UITableViewController <UISearchBarDelegate, UITableViewDelegate, CLLocationManagerDelegate>
@property (nonatomic, assign) id <ArticleDelegate> delegate;
@property (nonatomic, strong) NSString * currentName;
//uses on start of application
-(NSString *)getDefaultArticle;
-(NSString *)updateView:(NSString *)htmlId;
-(void)killKeyboard;
-(void)loadGuideAndPushToNavigationController:(NSString *)url;
@end
