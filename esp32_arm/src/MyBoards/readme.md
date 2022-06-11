# Board is a factory, Can provide component for consumers.
* Use Board::Get_a_kind_of_component() to get it.



# MCP23018 vs MCP23017
* internal pullup. 
  * MCP23018 support 100uA maximum.
  * MCP23017 is greater than MCP23018.
* Output
  * MCP23018 is OD output. Can drive ?? mA per port. 
  * MCP23017 need to check.   

# PWM channel assignment.
  ESP32PWM* pwmFactory(int pin) {
	for (int i = 0; i < NUM_PWM; i++)
		if (ESP32PWM::ChannelUsed[i] != NULL) {
			if (ESP32PWM::ChannelUsed[i]->getPin() == pin)
				return ESP32PWM::ChannelUsed[i];
		}
	return NULL;
}