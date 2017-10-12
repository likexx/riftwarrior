//
//  NSData+Encryption.h
//  tdgame
//
//  Created by Like Zhang on 6/25/13.
//
//

#import <Foundation/Foundation.h>

@interface NSData (Encryption)

- (NSData *)AES256EncryptWithKey:(NSString *)key;

- (NSData *)AES256DecryptWithKey:(NSString *)key;

- (NSString *)DecimalString;

+ (NSData*)dataWithDecimalString:(const char*)pszData withLength:(int)length;

@end
