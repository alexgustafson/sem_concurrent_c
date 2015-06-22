#ifndef TCP_SERVER_H_INCLUDED
#define TCP_SERVER_H_INCLUDED

#ifdef __cplusplus  // PITA : http://stackoverflow.com/questions/9334650/linker-error-calling-c-function-from-objective-c
extern "C" {
#endif
    


int startserver();
void stopserver();
    
int server_running();


#ifdef __cplusplus
}
#endif

#endif  // TCP_SERVER_H_INCLUDED
