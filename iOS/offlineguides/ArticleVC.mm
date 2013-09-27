#import "ArticleVC.h"
#import "GuideVC.h"
#import "GuideCell.h"
#import "AppDelegate.h"

#import "MapsWithMeAPI.h"

#import "../../storage/storage.hpp"

#import "../../env/assert.hpp"


#define THUMBNAILSFOLDER @"/data/thumb/"
#define UN_SELECTEDCELLCOLOUR ([UIColor colorWithRed:40.f/255.f green:40.f/255.f blue:40.f/255.f alpha:1.f])
#define SELECTEDCELLCOLOUR ([UIColor colorWithRed:28.f/255.f green:28.f/255.f blue:28.f/255.f alpha:1.f])

@interface ArticleVC ()
{
  Storage m_storage;
  CLLocationCoordinate2D m_lastLocation;
  NSDate * m_lastLocationTime;
}

@property (nonatomic, strong) UISearchBar * searchBar;
@property (nonatomic, strong) CLLocationManager * locationManager;
@property (nonatomic, strong) NSString * currentName;
@property (nonatomic, strong) NSString * currentSubtitle;
@end

@implementation ArticleVC

- (void)onMapButtonClicked
{
  size_t const count = m_storage.GetResultsCount();
  NSMutableArray * pins = [[NSMutableArray alloc] initWithCapacity:count];

  for (size_t i = 0; i < count; ++ i)
  {
    ArticleInfo const & article = m_storage.GetResult(i);
	double lat, lon;
    // Do not check for redirects as they don't contain valid coordinates
    if (/*!article.IsRedirect() && */article.GetLatLon(lat, lon))
    {
      NSString * title = [NSString stringWithUTF8String:article.GetTitle().c_str()];
      NSString * pageId = [NSString stringWithUTF8String:article.GetUrl().c_str()];
      [pins addObject:[[MWMPin alloc] initWithLat:lat lon:lon title:title andId:pageId]];
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

    NSString * dataDir = [(AppDelegate *)[[UIApplication sharedApplication] delegate] getDataFolderNameWithSlashes];
    NSString * path = [[NSBundle mainBundle] pathForResource:@"index" ofType:@"dat" inDirectory:dataDir];
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
    [self setCurrentNameAndSubtitle:0];
    [self.tableView reloadData];
  }
  return self;
}

- (void)viewDidLoad
{
  [super viewDidLoad];
  [self.searchBar sizeToFit];
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
  {
    self.tableView.backgroundColor = [UIColor colorWithRed:51.f/255.f green:51.f/255.f blue:51.f/255.f alpha:1.f];
    self.tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
  }
  self.navigationItem.titleView = self.searchBar;
  UIBarButtonItem * btn = [[UIBarButtonItem alloc] initWithTitle:NSLocalizedString(@"Map", nil) style:UIBarButtonItemStyleDone target:self action:@selector(onMapButtonClicked)];
  [btn setTitleTextAttributes: @{
                                 UITextAttributeTextColor : [UIColor colorWithRed:253.f/255.f green:241.f/255.f blue:43.f/255.f alpha:1.f],
                                  UITextAttributeTextShadowColor : [UIColor clearColor]
                                }
                     forState:UIControlStateNormal];
  self.navigationItem.rightBarButtonItem = btn;
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

  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
  {
    if ([self.currentName isEqualToString:cell.mainTitile.text] && [self.currentSubtitle isEqualToString:cell.subTitile.text])
    {
      cell.contentView.backgroundColor = SELECTEDCELLCOLOUR;
      [cell setSelected];
    }
    else
    {
      cell.contentView.backgroundColor = UN_SELECTEDCELLCOLOUR;
      [cell setUnselected];
    }
  }

  return (UITableViewCell *)cell;
}

- (void)viewWillAppear:(BOOL)animated
{
  [super viewWillAppear:animated];
  [self.navigationController.navigationBar setBackgroundImage:[UIImage imageNamed:@"bg_search"] forBarMetrics:UIBarMetricsDefault];
  [self.navigationController.navigationBar setBackgroundImage:[UIImage imageNamed:@"bg_search"] forBarMetrics:UIBarMetricsLandscapePhone];
  [self.navigationController setNavigationBarHidden:NO animated:animated];
}

#pragma mark - Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
  [self.tableView deselectRowAtIndexPath:indexPath animated:YES];
  ArticleInfo const * info = [self infoByIndexPath:indexPath];
  NSString * url = [NSString stringWithUTF8String:info->GetUrl().c_str()];
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)
    [self loadGuideAndPushToNavigationController:url];
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
  if (m_storage.GetResultsCount() && [self.tableView numberOfRowsInSection:0])
    [self.tableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:0 inSection:0] atScrollPosition:UITableViewScrollPositionTop animated:YES];
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

-(NSString *)updateView:(NSString *)htmlId
{
  ArticleInfo const * articleInfo = m_storage.GetArticleInfoFromUrl([htmlId UTF8String]);
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
  {
    self.currentName = [NSString stringWithUTF8String:articleInfo->GetTitle().c_str()];
    self.currentSubtitle = [NSString stringWithUTF8String:m_storage.FormatParentName(*articleInfo).c_str()];
    [self.tableView reloadData];
  }
  return [NSString stringWithUTF8String:articleInfo->GetTitle().c_str()];
}

-(void)killKeyboard
{
  [self.searchBar resignFirstResponder];
}

-(void)setCurrentNameAndSubtitle:(int)currentElement
{
  if (currentElement < m_storage.GetResultsCount())
  {
    ArticleInfo const & info = m_storage.GetResult(static_cast<size_t>(currentElement));
    self.currentName = [NSString stringWithUTF8String:info.GetTitle().c_str()];
    self.currentSubtitle = [NSString stringWithUTF8String:m_storage.FormatParentName(info).c_str()];
  }
}

-(void)loadGuideAndPushToNavigationController:(NSString *)url
{
  GuideVC * vc = [[GuideVC alloc] init];
  [self.navigationController pushViewController:vc animated:YES];
  [vc loadPage:url];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
  return YES;
}

@end
