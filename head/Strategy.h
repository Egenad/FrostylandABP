#ifndef STRATEGY_H
#define STRATEGY_H

class Strategy {
	public:
		virtual ~Strategy(){}
	    virtual void execute(int) = 0;
	private:
};

#endif /* STRATEGY_H */