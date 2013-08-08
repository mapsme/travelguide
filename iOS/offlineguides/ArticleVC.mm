#import "ArticleVC.h"
#import "GuideVC.h"

#import "../../storage/storage.hpp"

#import "../../env/assert.hpp"


#define THUMBNAILSFOLDER @"/data/thumb/"

@interface ArticleVC ()
{
  Storage m_storage;
}

@property (nonatomic, strong) UISearchBar * searchBar;
@end

@implementation ArticleVC

- (id)initWithStyle:(UITableViewStyle)style
{
  self = [super initWithStyle:style];
  if (self)
  {
    _searchBar = [[UISearchBar alloc] init];
    self.searchBar.delegate = self;
    [self clearSearchBackGround];
    [self.searchBar setBackgroundColor:[UIColor colorWithRed:102.f/255.f
                                 green:102.f/255.f
                                  blue:102.f/255.f
                                 alpha:1.f]];
    self.tableView.tableHeaderView = self.searchBar;
    self.searchBar.text = @"";
    [self searchBar:self.searchBar textDidChange:@""];

    NSString * path = [[NSBundle mainBundle] pathForResource:@"index" ofType:@"dat" inDirectory:@"/data/"];
    m_storage.Load([path UTF8String]);
    m_storage.QueryArticleInfo(string());

    [self.tableView reloadData];
  }
  return self;
}

- (void)viewDidLoad
{
  [super viewDidLoad];
  [self.searchBar sizeToFit];
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
  return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
  return static_cast<NSInteger>(m_storage.GetResultsCount());
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
  static NSString * CellIdentifier = @"ArticleCell";
  UITableViewCell * cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];

  if (!cell)
    cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:CellIdentifier];
  
  ArticleInfo const * info = [self infoByIndexPath:indexPath];
  cell.textLabel.text = [NSString stringWithUTF8String:info->GetTitle().c_str()];

  string const & thumbnail = info->GetThumbnailUrl();
  size_t const pos = thumbnail.find_last_of(".");
  string const imageName = thumbnail.substr(0, pos);
  string const imageType = thumbnail.substr(pos + 1);
  NSLog(@"%@ %@", [NSString stringWithUTF8String:imageName.c_str()], [NSString stringWithUTF8String:imageType.c_str()]);
  NSString * imagePath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:imageName.c_str()] ofType:[NSString stringWithUTF8String:imageType.c_str()] inDirectory:THUMBNAILSFOLDER];

  cell.detailTextLabel.text = [NSString stringWithUTF8String:m_storage.FormatParentName(*info).c_str()];

  UIImage * image = [UIImage imageWithContentsOfFile:imagePath];
  UIImageView * uiview = [[UIImageView alloc] initWithImage:image];
  [uiview setFrame:CGRectMake(0, 0, 44, 44)];

  cell.accessoryView = uiview;
//  [cell.imageView setFrame:CGRectMake(cell.imageView.frame.origin.x, cell.imageView.frame.origin.y, cell.imageView.frame.size.height, 44)];

  return cell;
}

- (void)viewWillAppear:(BOOL)animated
{
  [super viewWillAppear:animated];
  [self.navigationController setNavigationBarHidden:YES animated:animated];
}

#pragma mark - Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
  // @todo Show details guide info
  [self.tableView deselectRowAtIndexPath:indexPath animated:YES];
  ArticleInfo const * info = [self infoByIndexPath:indexPath];

  GuideVC * vc = [[GuideVC alloc] init];
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)
    [self.navigationController pushViewController:vc animated:YES];
  NSString * url = [NSString stringWithUTF8String:info->GetUrl().c_str()];
  [vc loadPage:url];
  [self.delegate selectHtmlPageUrl:url];
}

- (void)scrollViewWillBeginDragging:(UIScrollView *)scrollView
{
  [self.searchBar resignFirstResponder];
}

#pragma mark - UISearchBarDelegate

- (void)searchBar:(UISearchBar *)searchBar textDidChange:(NSString *)searchText
{
  /// @todo add lat and lon to QueryInfos
  m_storage.QueryArticleInfo([searchText UTF8String]);
  [self.tableView reloadData];
}

#pragma mark - Utils methods
-(ArticleInfo const *)infoByIndexPath:(NSIndexPath *)indexPath
{
  return &m_storage.GetResult(static_cast<size_t>(indexPath.row));
}

-(NSString *)getDefaultArticle
{
  return [NSString stringWithUTF8String:m_storage.GetResult(0).GetUrl().c_str()];
}

-(void)clearSearchBackGround
{
  for (UIView * sub in self.searchBar.subviews)
    if ([sub isKindOfClass:NSClassFromString(@"UISearchBarBackground")])
        [sub removeFromSuperview];
}

@end
