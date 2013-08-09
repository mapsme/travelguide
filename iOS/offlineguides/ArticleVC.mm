#import "ArticleVC.h"
#import "GuideVC.h"
#import "GuideCell.h"

#import "MapsWithMeAPI.h"

#import "../../storage/storage.hpp"

#import "../../env/assert.hpp"


#define THUMBNAILSFOLDER @"/data/thumb/"

@interface ArticleVC ()
{
  Storage m_storage;
  CLLocationCoordinate2D m_lastLocation;
  NSDate * m_lastLocationTime;
}

@property (nonatomic, strong) UISearchBar * searchBar;
@property (nonatomic, strong) CLLocationManager * locationManager;
@end

@implementation ArticleVC

- (void)onShowMap:(id)button
{
  size_t const count = m_storage.GetResultsCount();
  NSMutableArray * pins = [[NSMutableArray alloc] initWithCapacity:count];

  for (size_t i = 0; i < count; ++ i)
  {
    ArticleInfo const & article = m_storage.GetResult(i);
    if (!article.IsRedirect() && article.IsValidCoordinates())
    {
      NSString * title = [NSString stringWithUTF8String:article.GetTitle().c_str()];
      NSString * pageId = [NSString stringWithUTF8String:article.GetUrl().c_str()];
      [pins addObject:[[MWMPin alloc] initWithLat:article.m_lat lon:article.m_lon title:title andId:pageId]];
    }
  }
  [MWMApi showPins:pins];
}

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
    self.searchBar.text = @"";
    [self searchBar:self.searchBar textDidChange:@""];

    NSString * path = [[NSBundle mainBundle] pathForResource:@"index" ofType:@"dat" inDirectory:@"/data/"];
    self.locationManager = [[CLLocationManager alloc] init];
    self.locationManager.desiredAccuracy = kCLLocationAccuracyHundredMeters;
    self.locationManager.distanceFilter = 100;
    self.locationManager.delegate = self;
    [self.locationManager startUpdatingLocation];
    m_storage.Load([path UTF8String]);
    if ([self isValidCoordinates])
      m_storage.QueryArticleInfo(string(), self.locationManager.location.coordinate.latitude, self.locationManager.location.coordinate.longitude);
    else
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

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
  return 62.0;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
  static NSString * CellIdentifier = @"ArticleCell";
  GuideCell * cell = (GuideCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];

  if (!cell)
    cell = [[GuideCell alloc] initWithReuseIdentifier:CellIdentifier];

  ArticleInfo const * info = [self infoByIndexPath:indexPath];
  cell.mainTitile.text = [NSString stringWithUTF8String:info->GetTitle().c_str()];

  string const & thumbnail = info->GetThumbnailUrl();
  size_t const pos = thumbnail.find_last_of(".");
  string const imageName = thumbnail.substr(0, pos);
  string const imageType = thumbnail.substr(pos + 1);
  NSString * imagePath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:imageName.c_str()] ofType:[NSString stringWithUTF8String:imageType.c_str()] inDirectory:THUMBNAILSFOLDER];

  cell.subTitile.text = [NSString stringWithUTF8String:m_storage.FormatParentName(*info).c_str()];

  UIImage * image = [UIImage imageWithContentsOfFile:imagePath];
  cell.mainImage.image = image;

  return (UITableViewCell *)cell;
}

- (void)viewWillAppear:(BOOL)animated
{
  [super viewWillAppear:animated];
  [self.navigationController.navigationBar setBackgroundImage:[UIImage imageNamed:@"bg_search"] forBarMetrics:UIBarMetricsDefault];
  [self.navigationController.navigationBar setBackgroundImage:[UIImage imageNamed:@"bg_search"] forBarMetrics:UIBarMetricsLandscapePhone];
  [self.navigationController setNavigationBarHidden:NO animated:animated];
  self.navigationItem.titleView = self.searchBar;
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
  [self killKeyboard];
}

#pragma mark - UISearchBarDelegate

- (void)searchBar:(UISearchBar *)searchBar textDidChange:(NSString *)searchText
{
  if ([self isValidCoordinates])
    m_storage.QueryArticleInfo([searchText UTF8String], self.locationManager.location.coordinate.latitude, self.locationManager.location.coordinate.longitude);
  else
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

- (void)locationManager:(CLLocationManager *)manager didUpdateToLocation:(CLLocation *)newLocation fromLocation:(CLLocation *)oldLocation
{
  [self.locationManager stopUpdatingLocation];
}

-(BOOL)isValidCoordinates
{
  BOOL isValid = (self.locationManager.location != nil);
  if ([self.locationManager.location.timestamp timeIntervalSinceNow] > -1500.0)
    [self.locationManager startUpdatingLocation];
  return isValid;
}

-(NSString *)getArticleName:(NSString *)htmlId
{
  ArticleInfo const * tmp =  m_storage.GetTitleFromUrl([htmlId UTF8String]);
  return [NSString stringWithUTF8String:tmp->GetTitle().c_str()];
}

-(void)killKeyboard
{
  [self.searchBar resignFirstResponder];
}

@end
