#pragma once

#include "remotable_base.h"

class RemoteVar_Masterbase: public RemotableVar_Base{
	public:

	protected:
        void _PublishValue(char* new_value);

};