#if !defined _PWM_H
#define _PWM_H

#include "common.h"

#define PWM_TOP	(999)

void pwm_init(void);
void pwm_set_value(int, int);
int pwm_get_value(int);
void pwm_off(int);
void pwm_on(int);
int pwm_isrun(int);

#endif /* _PWM_H */
