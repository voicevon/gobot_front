#pragma once


class SyncGcodeRunnerBase{
    public:
        virtual void Start();
        virtual bool IsDone();

};