#ifndef COMMUNICATIONS_H
#define	COMMUNICATIONS_H

void usartTask(void);
void sendString(char *str);
void sendPString(const char *str);
void setupCommunications(void);
void disableRead(void);
void enableRead(void);
int readSensors(void);

#endif	/* COMMUNICATIONS_H */

