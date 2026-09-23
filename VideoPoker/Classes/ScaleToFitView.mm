//
//  ScaleToFitView.m
//  VideoPoker
//

#import "ScaleToFitView.h"

/* Re-render text and custom-drawn views at the scaled resolution so they
   stay sharp instead of being bitmap-stretched by the transform */
static void
setContentScale(UIView* view, CGFloat contentScale)
{
    static IMP baseDrawRect = [UIView instanceMethodForSelector: @selector(drawRect:)];
    if ([view isKindOfClass: [UILabel class]] ||
        [view methodForSelector: @selector(drawRect:)] != baseDrawRect)
    {
        if (view.contentScaleFactor != contentScale)
        {
            view.contentScaleFactor = contentScale;
            [view setNeedsDisplay];
        }
    }
    for (UIView* child in view.subviews)
    {
        setContentScale(child, contentScale);
    }
}

@implementation ScaleToFitView

@synthesize designView = _designView;

+ (ScaleToFitView*)
installInView: (UIView*) rootView
designSize: (CGSize) designSize
{
    /* rootView may already have been resized to the screen; shrink it back to
       the nib size so the children's autoresizing puts them where designed */
    CGRect rootFrame = rootView.frame;
    rootView.frame = CGRectMake(rootFrame.origin.x, rootFrame.origin.y, designSize.width, designSize.height);

    UIView* designView = [[UIView alloc] initWithFrame: CGRectMake(0.0, 0.0, designSize.width, designSize.height)];
    designView.backgroundColor = [UIColor clearColor];
    for (UIView* child in [[rootView.subviews copy] autorelease])
    {
        [designView addSubview: child];
    }

    ScaleToFitView* view = [[[ScaleToFitView alloc] initWithFrame: rootView.bounds] autorelease];
    view->_designView = designView;
    view->_designSize = designSize;
    view.backgroundColor = [UIColor clearColor];
    view.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    [view addSubview: designView];
    [rootView addSubview: view];

    rootView.frame = rootFrame;
    rootView.autoresizesSubviews = YES;     /* legacy nibs often have this off */
    view.frame = rootView.bounds;
    return view;
}

- (void)
dealloc
{
    [_designView release];
    [super dealloc];
}

- (void)
safeAreaInsetsDidChange
{
    [super safeAreaInsetsDidChange];
    [self setNeedsLayout];
}

- (void)
layoutSubviews
{
    [super layoutSubviews];

    CGRect available = UIEdgeInsetsInsetRect(self.bounds, self.safeAreaInsets);
    if (_designSize.width <= 0.0 || _designSize.height <= 0.0 || CGRectIsEmpty(available))
    {
        return;
    }

    CGFloat scale = MIN(CGRectGetWidth(available) / _designSize.width,
                        CGRectGetHeight(available) / _designSize.height);

    _designView.transform = CGAffineTransformIdentity;
    _designView.bounds = CGRectMake(0.0, 0.0, _designSize.width, _designSize.height);
    _designView.transform = CGAffineTransformMakeScale(scale, scale);
    _designView.center = CGPointMake(CGRectGetMidX(available),
                                     CGRectGetMinY(available) + _designSize.height * scale / 2.0);

    CGFloat screenScale = self.window ? self.window.screen.scale : [UIScreen mainScreen].scale;
    setContentScale(_designView, screenScale * scale);
}

@end
