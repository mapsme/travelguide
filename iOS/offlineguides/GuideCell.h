//
//  GuideCell.h
//  offlineguides
//
//  Created by Kirill on 08/08/2013.
//  Copyright (c) 2013 offlineguides. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface GuideCell : UITableViewCell
@property (nonatomic, strong) UIImageView * mainImage;
@property (nonatomic, strong) UILabel * mainTitile;
@property (nonatomic, strong) UILabel * subTitile;

- (id)initWithReuseIdentifier:(NSString *)reuseIdentifier;
@end
