#ifndef BAHA_BASE_H
#define BAHA_BASE_H

class SystemBase;

class BAHA_Base {
protected:
    SystemBase *sys;
    bool pauseSys;

public:
    BAHA_Base(SystemBase *_sys=0) : sys(_sys), pauseSys(false) { }
    virtual ~BAHA_Base() { }

    void sysSet(SystemBase *_sys) {
        sys = _sys;
    }
    SystemBase *sysBaseGet() {
        return sys;
    }

    void pauseSysSet(bool doPause) {
        pauseSys = doPause;
    }
    bool pauseSysGet() {
        return pauseSys;
    }
};

class BAHA_Void : public BAHA_Base {
protected:
    SystemBase *sys;
    bool pauseSys;

public:
    BAHA_Void(SystemBase *_sys=0) : BAHA_Base(_sys) { }
    virtual ~BAHA_Void() { }

    void log(const char */*c*/) { }
    void logLineEnd() { }
};


#endif // BAHA_BASE_H
