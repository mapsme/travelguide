#import <UIKit/UIKit.h>

@interface GuideCell : UITableViewCell
@property (nonatomic, strong) UIImageView * mainImage;
@property (nonatomic, strong) UILabel * mainTitile;
@property (nonatomic, strong) UILabel * subTitile;

- (id)initWithReuseIdentifier:(NSString *)reuseIdentifier;
- (void)setSelected;
- (void)setUnselected;

@end
