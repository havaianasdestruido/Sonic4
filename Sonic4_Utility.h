
// ポップアップが表示中かどうか
BOOL Sonic4_IsEnabledAlertView();
// 制限時間１分を知らせるポップアップ表示
void Sonic4_StartAlertView();
// 制限時間１分経過を知らせるポップアップ表示
void Sonic4_TimeupAlertView();
// ポータル側のサウンドON/OFF設定を取得
BOOL Sonic4_isSoundFlag();
// セガロゴのデモ用として使用するか
void Sonic4_SetLogoDemoFlag(BOOL flag);
BOOL Sonic4_GetLogoDemoFlag();
// セガロゴのデモを終了する
void Sonic4_SetLogoDemoEnd();
BOOL Sonic4_GetLogoDemoEnd();
void Sonic4_LogoDemoEnd();
void Sonic4_LogoDemoSoundPlay();

#ifdef __OBJC__
@protocol Sonic4_UtilityDelegate

- (void)logoEndAction;

@end

@interface Sonic4_Utility : UIView <UIAlertViewDelegate>
{
	id <Sonic4_UtilityDelegate> delegate;
	BOOL alertViewFlag;	
}
@property (nonatomic, assign)    id <Sonic4_UtilityDelegate> delegate;
@property (nonatomic, readwrite) BOOL alertViewFlag;

+ (void)create;
+ (void)release;
+ (void)setLogoEndAction:(id <Sonic4_UtilityDelegate>)del;

@end
#endif //__objc__