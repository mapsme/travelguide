#import "ArticleVC.h"
#import "GuideVC.h"

/// @todo Replace on storage.hpp
#import "../../storage/storage_builder.hpp"

#import "../../env/assert.hpp"


#define THUMBNAILSFOLDER @"/data/thumbnails/"

@interface ArticleVC ()
{
  /// @todo Replace on Storage
  Storage m_storage;
  vector<ArticleInfo> m_infos;
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
    m_storage.QueryArticleInfos(m_infos, "");

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
  return static_cast<NSInteger>(m_infos.size());
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
  static NSString * CellIdentifier = @"ArticleCell";
  UITableViewCell * cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];

  if (!cell)
    cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:CellIdentifier];
  
  ArticleInfo const * info = [self infoByIndexPath:indexPath];
  cell.textLabel.text = [NSString stringWithUTF8String:info->m_title.c_str()];

  size_t pos = info->m_thumbnailUrl.find_last_of(".");
  string imageName = info->m_thumbnailUrl.substr(0,pos);
  string imageType = info->m_thumbnailUrl.substr(pos+1);
  NSString * imagePath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:imageName.c_str()] ofType:[NSString stringWithUTF8String:imageType.c_str()] inDirectory:THUMBNAILSFOLDER];

  cell.detailTextLabel.text = [NSString stringWithUTF8String:m_storage.GetParentName(*info).c_str()];

  UIImage * image = [UIImage imageWithContentsOfFile:imagePath];
  cell.imageView.image = image;

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
  {
    BOOL rootVC = [self.navigationController.viewControllers count] == 1 ? YES : NO;
    [self.navigationController pushViewController:vc animated:YES];
    if (rootVC)
      self.loadedWebPages = [[NSMutableArray alloc] init];
    else
      [vc clearPreviosViews];
  }

  vc.webPages = self.loadedWebPages;
  [vc loadPage:[NSString stringWithUTF8String:info->m_url.c_str()]];
  [self.delegate selectHtmlPageUrl:[NSString stringWithUTF8String:info->m_url.c_str()]];
}

- (void)scrollViewWillBeginDragging:(UIScrollView *)scrollView
{
  [self.searchBar resignFirstResponder];
}

#pragma mark - UISearchBarDelegate

- (void)searchBar:(UISearchBar *)searchBar textDidChange:(NSString *)searchText
{
  //@todo add lat and lon to QueryInfos
  m_storage.QueryArticleInfos(m_infos, [searchText UTF8String]);
  [self.tableView reloadData];
}

#pragma mark - Utils methods
-(ArticleInfo const *)infoByIndexPath:(NSIndexPath *)indexPath
{
  size_t const index = static_cast<size_t>(indexPath.row);
  CHECK(index < m_infos.size(), ("Index is too big"));
  return &m_infos[index];
}

-(NSString *)getDefaultArticle
{
  return [NSString stringWithUTF8String:m_infos[0].m_url.c_str()];
}

-(void)clearSearchBackGround
{
  for (UIView * sub in self.searchBar.subviews)
    if ([sub isKindOfClass:NSClassFromString(@"UISearchBarBackground")])
        [sub removeFromSuperview];
}

@end
