#import <UIKit/UIKit.h>

@protocol ArticleDelegate <NSObject>
-(void)selectHtmlPageUrl:(NSString *)url;
@end

@interface ArticleVC : UITableViewController <UISearchBarDelegate, UITableViewDelegate>
@property (nonatomic, assign) id <ArticleDelegate> delegate;
@property (nonatomic, strong) NSMutableArray * loadedWebPages;
//uses on start of application
-(NSString *)getDefaultArticle;
@end
