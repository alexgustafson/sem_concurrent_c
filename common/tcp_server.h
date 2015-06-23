#ifndef TCP_SERVER_H_INCLUDED
#define TCP_SERVER_H_INCLUDED

#ifdef __cplusplus  // PITA : http://stackoverflow.com/questions/9334650/linker-error-calling-c-function-from-objective-c
extern "C" {
#endif
    
    typedef int (*join_cb)(void);
    typedef int (*leave_cb)(void);
    typedef int (*take_cb)(int, int, int);
    typedef int (*size_cb)();
    typedef int (*status_cb)(int, int);
    typedef int (*winner_cb)();
    
    void register_callbacks(join_cb jcb, leave_cb lcb, take_cb tcb, size_cb scb, status_cb stcb, winner_cb wcb);

    int startserver(int port);
    void stopserver();
    
    int server_running();


#ifdef __cplusplus
}
#endif

#endif  // TCP_SERVER_H_INCLUDED
