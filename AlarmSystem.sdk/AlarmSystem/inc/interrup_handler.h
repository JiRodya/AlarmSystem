#ifndef INTERRUPT_HANDLER_H
#define INTERRUPT_HANDLER_H

int Init_GIC();
int Configure_GIC();
void ExceptionInit();
void EnableInts();
void DisableIntr();

#endif // end of INTERRUPT_HANDLER_H
