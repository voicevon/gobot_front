#pragma once

#include "mark_reader_base.h"
class MarkReaderRfid: public MarkReaderBase{
    int ReadMark() override;

};