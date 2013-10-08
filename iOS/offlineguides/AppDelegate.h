#import <UIKit/UIKit.h>

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow * window;

//returns data
-(NSString *)getDataFolderName;
//returns /data/
-(NSString *)getDataFolderNameWithSlashes;
@end
