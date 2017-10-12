//
//  KKIAPHelper.m
//  tdgame
//
//  Created by Like Zhang on 4/22/13.
//
//

#import "KKIAPHelper.h"
#import <StoreKit/StoreKit.h>
#import "KKIAPWrapper.h"

#define PRODUCT_099 "KKRiftLordGems099"
#define PRODUCT_199 "KKRiftLordGems199"
#define PRODUCT_399 "KKRiftLordGems399"
#define PRODUCT_799 "KKRiftLordGems799"

@implementation KKIAPHelper

@synthesize products;

+ (KKIAPHelper*)sharedInstance {
    
    static dispatch_once_t once;
    static KKIAPHelper* sharedInstance;
    
    dispatch_once(&once, ^{
       
        NSSet* productIdentifiers = [NSSet setWithObjects:
                                     [NSString stringWithUTF8String:PRODUCT_099],
                                     [NSString stringWithUTF8String:PRODUCT_199],
                                     [NSString stringWithUTF8String:PRODUCT_399],
                                     [NSString stringWithUTF8String:PRODUCT_799],
                                     nil];
        sharedInstance = [[self alloc] initWithProductIdentifiers:productIdentifiers];
        
    });
 
    return sharedInstance;
}

-(void)loadProducts {

    [self requestProductsWithCompletionHandler:^(BOOL success, NSArray *skProducts) {
        if (success) {
            products = [[NSMutableDictionary alloc] init];
            
            NSNumberFormatter *numberFormatter = [[NSNumberFormatter alloc] init];
            [numberFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
            [numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];

            for (SKProduct * skProduct in skProducts) {
                [products setObject:skProduct forKey:skProduct.productIdentifier];
                
                NSLog(@"Found product: %@ %@ %0.2f",
                      skProduct.productIdentifier,
                      skProduct.localizedTitle,
                      skProduct.price.floatValue);
                
                IAPItem item;
                item.productIdentifer = [skProduct.productIdentifier UTF8String];
                item.title = [skProduct.localizedTitle UTF8String];
                item.description = [skProduct.localizedDescription UTF8String];
                
                [numberFormatter setLocale:skProduct.priceLocale];
                NSString *stringPrice = [numberFormatter stringFromNumber:skProduct.price];
                
                item.gems = 0;
                item.coins = 0;
                item.price = [stringPrice UTF8String];
                NSLog(@"%@\nprice %@", skProduct.priceLocale.localeIdentifier, stringPrice);
                
                if (item.productIdentifer == PRODUCT_099)
                {
                    item.gems = 100;
                }
                else if (item.productIdentifer == PRODUCT_199)
                {
                    item.gems = 300;
                }
                else if (item.productIdentifer == PRODUCT_399)
                {
                    item.gems = 700;
                }
                else if (item.productIdentifer == PRODUCT_799)
                {
                    item.gems = 1400;
                    item.coins = 5000;
                }
                
                KKIAPWrapper::addItem(item);
                
            }

            [numberFormatter release];
            
        }
        else {
            NSLog(@"Failed to get product");
        }
    }];
}


-(void)buyProductByIdentifier:(NSString *)productIdentifier {
    SKProduct* product = [[self products] objectForKey:productIdentifier];
    
    SKPayment * payment = [SKPayment paymentWithProduct:product];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

@end
