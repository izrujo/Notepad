#ifndef _CNTCOMMANDFACTORY_H
#define _CNTCOMMANDFACTORY_H

class TextEditingForm;
class CNTCommand;

class CNTCommandFactory {
public:
	CNTCommandFactory(TextEditingForm* textEditingForm = 0);
	CNTCommandFactory(const CNTCommandFactory& source);
	~CNTCommandFactory();
	CNTCommandFactory& operator=(const CNTCommandFactory& source);

	CNTCommand* Make(int uID);
private:
	TextEditingForm* textEditingForm;
};

#endif //_CNTCOMMANDFACTORY_H