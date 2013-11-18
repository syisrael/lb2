#ifndef COMMUNICATIONS_H
#define	COMMUNICATIONS_H

void usartTask(void);
void sendString(char *str);
void sendPString(char *str);
void setupCommunications(void);

#endif	/* COMMUNICATIONS_H */

