//
//  ReturnViewController.mm
//  VideoPoker
//
//  Created by Mike McQueen on 8/10/10.
//  Copyright 2010 Amazon.com. All rights reserved.
//

#import "Platform.h"
#import "ReturnViewController.h"


@implementation ReturnViewController

@synthesize delegate;
@synthesize yourEvLabel;
@synthesize bestEvLabel;
@synthesize invalidHandLabel;
@synthesize detailButton = _detailButton;

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
*/

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)
setYourEv: (double) ev;
{
    [self setLabel: yourEvLabel ev: 100.0 * (ev / 5.0)];
}

- (void)
setBestEv: (double) ev;
{
    [self setLabel: bestEvLabel ev: 100.0 * (ev / 5.0)];
}

- (void)
setLabel: (UILabel*) label
ev:       (double)   ev
{
    stringstream ss;
    ss.precision(2);
    ss << fixed << ev;
    label.text = [NSString stringWithFormat:@"%s%%", ss.str().c_str()];
}

- (void)
setInvalid: (BOOL) invalid
{
    if (((YES == invalid) && (YES == invalidHandLabel.hidden)) ||
        ((NO == invalid) && (NO == invalidHandLabel.hidden)))
    {
        for (UIView* child in self.view.subviews)
        {
            child.hidden = invalid;
            [child setNeedsDisplay];
        }
        invalidHandLabel.hidden = !invalid;
    }
}

- (IBAction)
pressDetailButton: (id) sender
{
    [delegate returnView: self.view detailButtonWasPressed: (UIButton*)sender];
}

////////////////////////////////////////////////////////////////////////////////

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc
{
    [super dealloc];
}


@end
