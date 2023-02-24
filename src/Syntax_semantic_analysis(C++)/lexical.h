#include<iostream>
#include<cstring>
#include<string>
#include<map>
#include<vector>
#include<fstream>
#include<stdlib.h>
using namespace std;
typedef pair<int,int>ii;
	struct iip
	{
		ii val;ii pos;
		iip(ii a,ii b)
		{
			val=a;pos=b;
		}
	};
class compiler
{
public:
	
	map<string,int>kw;
	map<string,int>idx;
	map<string,int>vc;
	int total;
	map<int,string>name;
	//第一个数据存储该符号在程序中出现的次序
	//第二个数据用来存储该符号的内容
	vector<iip>vec;
	vector<ii>error1;
	vector<ii>error2;
	void initialize()
	{
	string keyword[35]={"and","array","begin","bool","call","case","char","constant","dim","do",
            "else","end","false","for","if","input","integer","not","of","or",
            "output","procedure","program","read","real","repeat","set","stop","then","to",
            "true","until","var","while","write"
        };
	string t[22]={
            "(",")","*","*/","+",",","-",".","..","/","/*",":",":=",
            ";","<","<=","<>","=",">",">=","[","]"
        };
	string st[15]={
            "(",")","*","+",",","-",".","/",":",
            ";","<","=",">","[","]"
        };
	for(int i=0;i<35;i++) kw[keyword[i]]=i+1;
	for(int i=0;i<22;i++) kw[t[i]]=i+39;
	for(int i=0;i<15;i++) vc[st[i]]=i+1;
	total=0;
     }
	bool ischar(char a)
	{return (a>='a'&&a<='z')||(a>='A'&&a<='Z');}
	bool isnum(char a)
	{return a>='0'&&a<='9';}
	bool validchar(char a)
	{
		if(ischar(a))
			return 1;
		else if(isnum(a))
			return 1;
		else 
		{
			string x;
			x.push_back(a);
			if(vc.find(x)!=vc.end())
				return 1;
			else return 0;
		}
	}
	int judge(string s)
	{
		if(kw.find(s)!=kw.end())return kw[s];
		int sz=s.size();
		if(!ischar(s[0]));
		else
		{
			for(int i=1;i<sz;i++)
				if((!ischar(s[i]))&&(!isnum(s[i])))
					return 61;
			if(!idx[s])
            {
                idx[s]=++total;
				name[total]=s;
                
            }
			return 36;
		}
		if(isnum(s[0]))
			{for(int i=1;i<sz;i++)
				if(!isnum(s[i]))
					return 61;
			 if(!idx[s])
            {
                idx[s]=++total;
                name[total]=s;
            }
		     return 37;
		    }
		if(s[0]=='\'')
		{
			for(int i=1;i<sz-1;i++)
				if(s[i]=='\'')
					return 61;
			if(s[sz-1]!='\'')
				return 62;
			else
			{
				for(int i=1;i<sz-1;i++)
					if(!validchar(s[i]))
						return 61;
				if(!idx[s])
            {
                idx[s]=++total;
                name[total]=s;
            }
				return 38;
			}
		}
		return 61;
	}
	void judgehelp(string a,int line,int pos)
	{
		int id=judge(a);
		int id1=0;
		if(id>=36&&id<=38)id1=idx[a];
		vec.push_back(iip(ii(id,id1),ii(line,pos)));
	}
	void showtitle()
	{
		cout<<"author：2020级计联姜融青 202030551259 "<<endl<<"张杰铖 202030443394"<<endl<<endl;;
//		cout<<"please choose test text file："<<endl;
//		cout<<"1.TEST1  2.TEST2  3.TEST4  4.TEST5"<<endl;
	}
	void begin()
	{
	    char filename[105];
		string s;
		int line=0;
		initialize();
		showtitle();
//		int n;cin>>n;
//		switch(n)
//		{
//		case 1:strcpy(filename,"./TEST1.txt");break;
//		case 2:strcpy(filename,"./TEST2.txt");break;
//		case 3:strcpy(filename,"./TEST4.txt");break;
//		case 4:strcpy(filename,"./TEST5.txt");break;
//		}
		strcpy(filename,"./TEST2.txt");
        freopen(filename,"r",stdin);
		while(getline(cin,s))
        {
            line++;
            int sz=s.size();
            string tp;
            int pos=0;
            for(int i=0;i<sz;i++)
            {
                if(s[i]!=' ')
                {
                    string x;
                    int num=0;
                    x.push_back(s[i]);
                    if(kw.find(x)!=kw.end()) num=kw[x];
                    if(num>=39)
                    {
                        if(!tp.empty()) judgehelp(tp,line,++pos),tp.clear();
                        if(i!=sz-1)
                        {
                            i++;
                            if(num==41 && s[i]=='/') vec.push_back(iip(ii(42,0),ii(line,++pos)));
                            else if(num==46 && s[i]=='.') vec.push_back(iip(ii(47,0),ii(line,++pos)));
                            else if(num==48 && s[i]=='*') vec.push_back(iip(ii(49,0),ii(line,++pos)));
                            else if(num==50 && s[i]=='=') vec.push_back(iip(ii(51,0),ii(line,++pos)));
                            else if(num==53 && s[i]=='=') vec.push_back(iip(ii(54,0),ii(line,++pos)));
                            else if(num==53 && s[i]=='>') vec.push_back(iip(ii(55,0),ii(line,++pos)));
                            else if(num==57 && s[i]=='=') vec.push_back(iip(ii(58,0),ii(line,++pos)));
                            else
                            {
                                i--;
                                vec.push_back(iip(ii(num,0),ii(line,++pos)));
                            }
                        }
                        else vec.push_back(iip(ii(num,0),ii(line,++pos)));
                    }
                    else tp.push_back(s[i]);
                }
                else if(!tp.empty()) judgehelp(tp,line,++pos),tp.clear();
            }
            if(!tp.empty()) judgehelp(tp,line,++pos),tp.clear();
        }
	}
	void output(int &u)
	{
		int sz=vec.size();
		for(int i=0;i<sz;i++)
        {
            if(vec[i].val.first==61)
                error1.push_back(vec[i].pos);
            if(vec[i].val.first==62)
                error2.push_back(vec[i].pos);    
        }
		int sz1=error1.size();
		int sz2=error2.size();
		int mark=-1;
		int e3=0;
		//判断是否有右单注释并记录在mark中
		for(int i=0;i<sz;i++)
		{
			if(vec[i].val.first==49 && e3==0)
                e3=1;
            if(vec[i].val.first==42)
            {
                if(e3==1) e3=0;
                else
                {
                    mark=i;
                    break;
                }
		    }
		}
		
		if(sz1)
		{
			cout<<"wrong1（Illegal input）happens in： \n";
			for(int i=0;i<sz1;i++)
                cout<<"the"<<error1[i].first<<"line"<<' '<<"the "<<error1[i].second<<"word"<<endl;
		}
		if(sz2)
		{
			cout<<"wrong2（need ’）happens in：\n";
			for(int i=0;i<sz2;i++)
                cout<<"the"<<error2[i].first<<"line"<<' '<<"the "<<error2[i].second<<"word"<<endl;
		}
		if(mark!=-1)
        {
            cout<<"wrong3（begin with */）happens in：\n";
            cout<<"the"<<vec[mark].pos.first<<"line"<<' '<<"the "<<vec[mark].pos.second<<"word"<<endl;
        }
		if(e3==1)
        {
            cout<<"wrong3（need a */）happens in \n";
        }
		if(sz1||sz2||mark!=-1||e3==1)
			u=0;
		else u=1;
	}
	vector<ii> lexicalresult1()
	{
		vector<ii>rett;
            int cnt=0;
            int st=0;
            for(int i=0;i<vec.size();i++)
            {
                if(vec[i].val.first==49) st=1;
                if(vec[i].val.first==42)
                {
                    st=0;
                    continue;
                }
                if(st) continue;
                ii temp(vec[i].val.first,-1);
                if(vec[i].val.second==0);
                else temp.second=vec[i].val.second;
                rett.push_back(temp);
            }
            return rett;
	}
	vector<iip> lexicalresult2()
	{
		vector<iip>ret;
            int cnt=0;
            int st=0;
            for(int i=0;i<vec.size();i++)
            {
                if(vec[i].val.first==49) st=1;
                if(vec[i].val.first==42)
                {
                    st=0;
                    continue;
                }
                if(st) continue;
				iip temp(ii(vec[i].val.first,-1),ii(vec[i].pos.first,vec[i].pos.second));
                if(vec[i].val.second==0);
                else temp.val.second=vec[i].val.second;
                ret.push_back(temp);
            }
            return ret;
	}
};
