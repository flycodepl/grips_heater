uint8_t key1_lock, key2_lock;

void key_press( uint8_t * klock, volatile uint8_t * KPIN, uint8_t key_mask, void (*kfun)(void) );

void key_press(uint8_t * klock, volatile uint8_t * KPIN, uint8_t key_mask, void (*kfun)(void) ) {
  register uint8_t key_press = (*KPIN & key_mask);
  if( !*klock && !key_press ) {
    *klock=1;
    if(kfun) kfun();
  } else if( *klock && key_press ) (*klock)++;
}
