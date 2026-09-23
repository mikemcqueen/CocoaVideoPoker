//
//  ScaleToFitView.h
//  VideoPoker
//
//  Scales a fixed-size (320pt wide) legacy layout uniformly so it fills the
//  width of the screen, centered and clear of the navigation and tab bars.
//

@interface ScaleToFitView :
    UIView
{
    UIView*     _designView;
    CGSize      _designSize;
}

/* Moves all current subviews of rootView into a design view of designSize
   (the size the nib was laid out for), and scales that to fit rootView.
   Call at the end of viewDidLoad, after all subviews have been added. */
+ (ScaleToFitView*) installInView: (UIView*) rootView designSize: (CGSize) designSize;

@property (nonatomic, readonly) UIView* designView;

@end
