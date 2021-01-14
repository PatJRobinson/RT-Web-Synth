/***** dataIn.hpp *****/
class InputData 
{
	public: 
	
	void write(int buffer[3]);
	void print(int typeIn, int channelIn);
	int getVal(int typeIn, int channelIn);
	
	private:
	
	int buf_[256][128];
	
	
};