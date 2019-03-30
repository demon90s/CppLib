#pragma once

class IMsgQCallback {
public:
    virtual ~IMsgQCallback() {}

    virtual void OnRecv(const char* data, int len) = 0;
};
