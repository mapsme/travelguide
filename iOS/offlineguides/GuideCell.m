#import "GuideCell.h"

@implementation GuideCell

- (id)initWithReuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:UITableViewCellStyleDefault reuseIdentifier:reuseIdentifier];
    if (self)
    {
      _mainImage = [[UIImageView alloc] init];
      _mainTitile = [[UILabel alloc] init];
      _subTitile =  [[UILabel alloc] init];
      self.mainTitile.backgroundColor = [UIColor clearColor];
      self.mainTitile.font = [UIFont fontWithName:@"Helvetica-Regular" size:25];
      self.subTitile.backgroundColor = [UIColor clearColor];
      self.subTitile.font = [UIFont fontWithName:@"Helvetica-Light" size:13];
      self.subTitile.textColor  = [UIColor colorWithRed:51.f/255.f
                                                  green:51.f/255.f
                                                   blue:51.f/255.f
                                                  alpha:1.f];
      [self.contentView addSubview:self.mainImage];
      [self.contentView addSubview:self.mainTitile];
      [self.contentView addSubview:self.subTitile];
    }
    return self;
}


- (void)layoutSubviews
{
  [super layoutSubviews];
  double padding = 6 + 50 + 12;
  [self.mainImage setFrame:CGRectMake(6, 6, 50, 50)];
  [self.mainTitile setFrame:CGRectMake(padding, 8, self.frame.size.width - padding, 25)];
  [self.subTitile setFrame:CGRectMake(padding, 30, self.frame.size.width - padding, 22)];
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];
}

@end
