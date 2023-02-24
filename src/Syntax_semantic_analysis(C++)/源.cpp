#include"lexical.h"
#include<iostream>
#include<string.h>
#include <algorithm>
#include<cstring>
#include<map>
#include<vector>
#include<fstream>
#include<stdlib.h>
#include<sstream>
using namespace std;

template<typename _T>
string to_string(const _T& t) {
	ostringstream os;
	os << t;
	return os.str();
}
class grammatical_complier
{
public:
	string numtostr(int num)
	{
		string rett;
		while(num)
		{
			rett.push_back(num%10+'0');
			num/=10;
		}
		reverse(rett.begin(),rett.end());
		return rett;
	}
	void error()
	{}
	struct forth
	{
		string a,b,c,d;
		forth(string A,string B,string C,string D)
		{
			a=A;b=B;c=C;d=D;
		}
	};
	static int nend;
	string regstr()
	{
		return "T"+numtostr(++nend);
	}
	compiler com;
	vector<ii>vec;
	vector<iip>vecp;
	vector<forth>result;
	vector<int>tag;
	vector<int>tagint;
	vector<int>tagbool;
	int cur;
	void output()
	{
		int sz=result.size();
		for(int i=0;i<sz;i++)
            cout<<'('<<i<<")("<<result[i].a<<','<<result[i].b<<','<<result[i].c<<','<<result[i].d<<")\n";
	}
	void match(int num)
    {
        if(vec[cur].first==num) cur++;
        else error();
    }
	void addto(string a,string b,string c,string d)
    {
        result.push_back(forth(a,b,c,d));
    }
	void run()
	{
		com.begin();
		vec=com.lexicalresult1();
		vecp=com.lexicalresult2();
		cur=0;
		sen_prog();
		addto("sys","-","-","-");
        cout<<'\n';
        output();
	}
	void sen_prog()
	{
		match(23);
		match(36);
		match(52);
		addto("Program",com.name[vec[cur-2].second],"-","-");   
		if(vec[cur].first==33) sen_var();  //变量说明语句子程序
        if(vec[cur].first==3) sen_comby();   //复合语句子程序
        else error();
    }
	void sen_var()
	{
		match(33);
		sen_varhelp();
	}
	void sen_varhelp()
	{
		int t1=tag.size();     //tag用于存储词法分析出现的标识符
		if(vec[cur].first==36)
			sen_vartag();
		else error();
		match(50);
		int t2=tag.size();
		sen_type(t1,t2);     //对标识符进行分类
		match(52);
		if(vec[cur].first==36)sen_varhelp();
	}
	void sen_vartag()
	{
		match(36);
		tag.push_back(vec[cur-1].second);
		if(vec[cur].first==44)
		{
			match(44);
			sen_vartag();
		}
	}
	void sen_type(int x,int y)
	 {
		 //对标识符类型分类
        if(vec[cur].first==4 || vec[cur].first==7 || vec[cur].first==17)
		{
			if(vec[cur].first==4)
			{
				for(int i=x;i<y;i++)
					tagbool.push_back(tag[i]);       
			}
			else if(vec[cur].first==17)
			{
				for(int i=x;i<y;i++)
					tagint.push_back(tag[i]);        
			}
			cur++;
		}
        else error();
    }
	void sen_comby()      //复合语句
	{
		match(3);
		sen_sentable();
		match(12);
	}
	void sen_sentable()      //语句表
	{
		sen_sen();
		if(vec[cur].first==52)
        {
            cur++;
            sen_sentable();
        }
	}
	void sen_sen()
	{
		if(vec[cur].first==36) sen_assignsen();          
        else if(vec[cur].first==15) sen_ifsen();         
        else if(vec[cur].first==34) sen_whilesen();      
        else if(vec[cur].first==26) sen_repeatsen();    
        else if(vec[cur].first==3) sen_comby();          
        else error();
	}
	void sen_assignsen()
	{
		match(36);
		//未定义就使用报错
		int flag=0;
		for(int i=0;i<tag.size();i++)
		{
			if(vec[cur-1].second==tag[i])
				flag=1;
		}
		if(!flag)
		{
			cout<<"Wrong!（Identifiers are used without being defined）happens inthe "<<vecp[cur-1].pos.first<<"line the"<<vecp[cur-1].pos.second<<"word"<<endl;
			exit(1);
		}
		match(51);
		string x=com.name[vec[cur-2].second];
		int k=vec[cur-2].second;
        string y=sen_ari_ex();
		//类型不匹配报错
		if(y=="true"||y=="false")
		{
			int flag=0;
			for(int i=0;i<tagbool.size();i++)
				if(k==tagbool[i])
					flag=1;
			if(!flag)
			{
				cout<<"Wrong!（Type mismatch）happens in the "<<vecp[cur-1].pos.first<<"line the"<<vecp[cur-1].pos.second<<"word"<<endl;
			exit(1);
			}
		}
		else 
			{
			int flag=0;
			for(int i=0;i<tagint.size();i++)
			{
				if(k==tagint[i])
					flag=1;
			}
			if(!flag)
			{
				cout<<"Wrong!（Type mismatch）happens in the "<<vecp[cur-1].pos.first<<"line the"<<vecp[cur-1].pos.second<<"word"<<endl;
			exit(1);
			}
		}
        addto(":=",y,"-",x);
	}
	string arifor(string A)
    {
        string tp;
        for(int i=0;i<A.size();i++)
        {
            tp.push_back(A[i]);
            if(A[i]=='+'||A[i]=='-'||A[i]=='*'||A[i]=='/')
            {
                string x=regstr();
                string y=arifor(A.substr(i+1));
                string op;op.push_back(A[i]);
                addto(op,A.substr(0,i),y,x);
                return x;
            }
        }
        return tp;
    }
	string sen_ari_ex()              
	{
		string x=sen_project();
		string y=sen_ari_exhelp();
		string z=arifor(x+y);
		return z;
	}
	string sen_project()
	{
		string x=sen_factor();
		string y=sen_projecthelp();
		string z=arifor(x+y);
		return z;
	}
	string sen_factor()
	{
		if(vec[cur].first==45)
        {
            ++cur;
            string x=sen_factor();
            string y=regstr();
            addto("-","minus",x,y);
            return y;
        }
        else return sen_ariplus();
	}
	string sen_ariplus()
	{
		if(vec[cur].first==36) cur++;
        else if(vec[cur].first==37) cur++;
        else if(vec[cur].first==39)
        {
            cur++;
            string x=sen_ari_ex();
            match(40);
            return x;
        }
		else if(vec[cur].first==13)
		{
			cur++;
			return "false";
		}
		else if(vec[cur].first==31)
		{
			cur++;
			return "true";
		}
		else if(vec[cur].first==29)
		{
			cur++;
			return "then";
		}
		else if(vec[cur].first==10)
		{
			cur++;
			return "do";
		}
		
        return com.name[vec[cur-1].second];
	}
	string sen_projecthelp()
	{
		if(vec[cur].first==41)
        {
            cur++;
            string tp1=sen_factor();
            string tp2=sen_projecthelp();
            return "*"+tp1+tp2;
        }
        else if(vec[cur].first==48)
        {
            cur++;
            string tp1=sen_factor();
            string tp2=sen_projecthelp();
            return "/"+tp1+tp2;
        }
        else return "";
	}
	string sen_ari_exhelp()
    {
        if(vec[cur].first==43)
        {
            cur++;
            string tp1=sen_project();
            string tp2=sen_ari_exhelp();
            return "+"+tp1+tp2;
        }
        else if(vec[cur].first==45)
        {
            cur++;
            string tp1=sen_project();
            string tp2=sen_ari_exhelp();
            return "-"+tp1+tp2;
        }
        else return "";
    }
	void sen_ifsen()                 //if表达式
	{
		int tcur=result.size();
        match(15);
		int flag1=0;
		if(vec[cur].first==37)
		{
			string a=com.name[vec[cur].second];
			if(a=="0")
			{flag1=1;addto("j","-","-","F");}
			else {flag1=1;addto("j","-","-","T");}
			cur++;
		}
		if(vec[cur].first==31||vec[cur].first==13)
		{
			if(vec[cur].first==31)
			{flag1=1;addto("j","-","-","T");}
			else {flag1=1;addto("j","-","-","F");}
			cur++;
		}
		if(vec[cur].first==36&&vec[cur].first==29)
		{
			string a=com.name[vec[cur].second];
			cur++;
			addto("jnz",a,"-","T");
			addto("j","-","-","F");
		}

		if(!flag1)sen_bool_ex();          
        if(vec[cur].first!=29)
		{
			cout<<"wrond!（without then behind if）happens in the"<<vecp[cur].pos.first<<"line the"<<vecp[cur].pos.second<<"word"<<endl;
			exit(1);
		}
		else cur++;
        int Ttrue=result.size();
        sen_sen();
        int Ffalse=result.size();
        if(vec[cur].first==11)
        {
            addto("j","-","-","-");
            Ffalse++;
            int temp=result.size()-1;
            match(11);
            sen_sen();
            result[temp].d=numtostr(result.size());
        }
        for(int i=tcur;i<result.size();i++)
        {
            if(result[i].d=="T") result[i].d=numtostr(Ttrue);
            if(result[i].d=="F") result[i].d=numtostr(Ffalse);
        }
	}
	//布尔表达式
	void sen_bool_ex()
	{
		sen_boolpro();
		sen_bool_exhelp();
	}
	//布尔项
	void sen_boolpro()
	{
		sen_boolfac();
		sen_boolprohelp();
	}
	//布尔因子
	void sen_boolfac()
	{
		if(vec[cur].first==18)
        {
            cur++;
            sen_boolfac();
        }
        else
            sen_boolthing();
	}
	//布尔量
	void sen_boolthing()
	{
		if(vec[cur].first==13 || vec[cur].second==31)
        {
            sen_boolconst();
        }
        else if(vec[cur].first==39)
        {
            match(39);
            sen_bool_ex();
            match(40);
        }
        else
        {
            string tp1=sen_ari_ex();
			//缺少表达式报错
			if(tp1=="then")
			{
				cout<<"worng!（No Boolean expressions after if）happens in the"<<vecp[cur].pos.first<<"line the"<<vecp[cur].pos.second-1<<"word"<<endl;
			exit(1);
			}
			if(tp1=="do")
			{
				cout<<"worng!（No Boolean expressions after while）happens in the"<<vecp[cur].pos.first<<"line the"<<vecp[cur].pos.second-1<<"word"<<endl;
			exit(1);
			}

            if(vec[cur].first>=53&&vec[cur].first<=58)
            {
                int tp=result.size();
                string op;
                if(vec[cur].first==53) op="<";
                if(vec[cur].first==54) op="<=";
                if(vec[cur].first==55) op="<>";
                if(vec[cur].first==56) op="=";
                if(vec[cur].first==57) op=">";
                if(vec[cur].first==58) op=">=";
                ++cur;
                addto("j"+op,tp1,"-","T");
                addto("j","-","-","F");
                string tp2=sen_ari_ex();
                result[tp].c=tp2;
                return;
            }
            addto("jnz",tp1,"-","T");
            addto("j","-","-","F");         //jnz改为j
        }
	}
	//布尔常量
	void sen_boolconst()
	{
        if(vec[cur].first==13)
        {
            cur++;
            addto("j","-","-","F");
        }
        else
        {
            cur++;
            addto("j","-","-","T");
        }
    }
	void sen_boolprohelp()
	{
		if(vec[cur].first==1)
        {
            cur++;
            result[result.size()-2].d=to_string(result.size());
			sen_boolfac();
			sen_boolprohelp();
        }
	}
	void sen_bool_exhelp()
	{
		if(vec[cur].first==20)
        {
            cur++;
            result[result.size()-1].d=to_string(result.size());
			sen_boolpro();
			sen_bool_exhelp();
        }
	}
	//while语句
	void sen_whilesen()
	{
		match(34);
		int tcur=result.size();
		int flag1=0;
		if(vec[cur].first==37)
		{
			string a=com.name[vec[cur].second];
			if(a=="0")
			{flag1=1;addto("j","-","-","F");}
			else {flag1=1;addto("j","-","-","T");}
			cur++;
		}
		if(vec[cur].first==36&&vec[cur].first==29)
		{
			string a=com.name[vec[cur].second];
			cur++;
			addto("jnz",a,"-","T");
			addto("j","-","-","F");
		}

		if(!flag1)sen_bool_ex();
        if(vec[cur].first!=10)
		{
			cout<<"wrong! （without do after while）happens in the"<<vecp[cur].pos.first<<"line the"<<vecp[cur].pos.second<<"word"<<endl;
			exit(1);
		}
		else cur++;
        int Ttrue=result.size();
        sen_sen();
		addto("j","-","-",numtostr(tcur));
        int Ffalse=result.size();
        for(int i=tcur;i<result.size();i++)
        {
            if(result[i].d=="T") result[i].d=numtostr(Ttrue);
            if(result[i].d=="F") result[i].d=numtostr(Ffalse);
        }
	}
	void sen_repeatsen()
	{
		int Ttrue=result.size();
        match(26);
        sen_sen();
        int tcur=result.size();
        if(vec[cur].first!=32)
		{
			cout<<"wrong!（without until after repeat）happens in"<<vecp[cur].pos.first<<"line the"<<vecp[cur].pos.second<<"word"<<endl;
			exit(1);
		}
		else cur++;
        sen_bool_ex();
        for(int i=tcur;i<result.size();i++)
        {
            if(result[i].d=="F") result[i].d=numtostr(Ttrue);
            if(result[i].d=="T") result[i].d=numtostr((int)result.size());
        }
	}
	bool errorplus()
	{
		int sz1=result.size();
		int sz2=vec.size();
		for(int i=0;i<sz2;i++)
		{
			if(vec[i].first==36)
			{
				string tp=com.name[vec[i].second];
				for(int j=0;j<sz1;j++)
				{

				}
			}
		}
		

	}
};
int grammatical_complier::nend=0;
int main()
{
	grammatical_complier gcom;
	gcom.run();

}
