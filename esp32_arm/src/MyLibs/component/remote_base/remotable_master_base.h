#pragma once

#include "remotable_base.h"

class RemoteVar_Masterbase: public RemotableVar_Base{
	public:

	protected:
        void _PublishValue(const char* new_value);

};