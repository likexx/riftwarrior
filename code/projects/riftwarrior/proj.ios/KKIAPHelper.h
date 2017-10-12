//
//  KKIAPHelper.h
//  tdgame
//
//  Created by Like Zhang on 4/22/13.
//
//

#import "IAPHelper.h"

@interface KKIAPHelper : IAPHelper {
    NSMutableDictionary* products;
}
@property (nonatomic, retain) NSMutableDictionary* products;

+ (KKIAPHelper*)sharedInstance;

- (void)loadProducts;
- (void)buyProductByIdentifier:(NSString *)productIdentifier;

@end
