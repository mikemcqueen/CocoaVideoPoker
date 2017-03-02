//
//  ReturnViewController.h
//  VideoPoker
//
//  Created by Mike McQueen on 8/10/10.
//  Copyright 2010 Amazon.com. All rights reserved.
//

@protocol ReturnViewControllerDelegate

- (void)
returnView: (UIView*) view
detailButtonWasPressed: (UIButton*) button;

@end

@interface ReturnViewController :
    UIViewController
{
@private
    id<ReturnViewControllerDelegate> delegate;

    UILabel* yourEvLabel;
    UILabel* bestEvLabel;
    UILabel* invalidHandLabel;
    UIButton* _detailButton;
}

@property(nonatomic, assign) IBOutlet id<ReturnViewControllerDelegate>  delegate;

@property(nonatomic, retain) IBOutlet UILabel*                          yourEvLabel;
@property(nonatomic, retain) IBOutlet UILabel*                          bestEvLabel;
@property(nonatomic, retain) IBOutlet UILabel*                          invalidHandLabel;
@property(nonatomic, retain) IBOutlet UIButton*                         detailButton;

- (void) setYourEv: (double) ev;
- (void) setBestEv: (double) ev;

- (void) setLabel: (UILabel*) label
               ev: (double) ev;

- (void) setInvalid: (BOOL) invalid;

- (IBAction) pressDetailButton: (id) sender; 

@end
