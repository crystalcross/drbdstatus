<?php

function drbd_get_info()
{

	global $lpos;


	function parsearray($typ,$lst)
	{
		global $lpos;

		$r=array();
		for($lpos+=1;( ($lpos<count($lst)) && ($lst[$lpos]!="}"));$lpos++)
			if (preg_match("/^(\S+):(\S*) *\{$/",$lst[$lpos],$m))
			{
				switch ($m[1])
				{
					case "connection":
						$e=parsearray($m[1],$lst);
						$n=str_replace('"','',$e['net']['_name']);
						$r['connection'][$n]=$e;
						break;
					default:
						$r[$m[1]]=parsearray($m[1],$lst);
						break;
				}
			}
			else if (preg_match("/^(.*)\;$/",$lst[$lpos],$m))
			{
				$lst[$lpos]=$m[1];
				$m=explode(":",$lst[$lpos]);
				switch ($m[0])
				{
					case "shared-secret":
						$m[1]="********";
						break;
					case "device":
						$e=explode(" ",$m[1]);
						$m[1]='"/dev/drbd'.$e[1].'"';
						break;
					case "_this_host":
					case "_remote_host":
						$e=explode(" ",$m[1]);
						$ip=array();
						$ip['ip']=$e[1];
						$ip['proto']=$e[0];
						$e=gethostbyaddr($e[1]);
						$e=explode(".",$e);
						$ip['host']=$e[0];
						$ip['port']=$m[2];
						$m[1]=$ip;
						break;
				}
				if (isset($r[$m[0]]))
				{
					if (!is_array($r[$m[0]]))
						$r[$m[0]]=array($r[$m[0]]);
					$r[$m[0]][]=$m[1];
				}
				else $r[$m[0]]=$m[1];
			}
		return($r);
	}
	function cleanup($a)
	{
		$e=array();
		if (is_array($a))
		{
			foreach($a as $c)
			{
				if (is_array($c))
				{
					$d=cleanup($c);
					if (isset($d)) $e[]=$d;
				}
				else
				{
					$d=cleanup($c);
					if (isset($d))
					{
						if (is_array($d))
							foreach($d as $f) $e[]=$f;
						else 
							$e[]=$d;
					}
				}
			}
		}
		else
		{
			$b=explode(";",$a);
			foreach($b as $c)
			{
				$c=preg_replace("/\#.*$/","",$c);
				$d=ltrim(rtrim($c));
				if ($d<>"") $e[]=$d;
			}
		}
		if ($e>1) return($e);
		if ($e==1) return($e[0]);
		return(undef);
	}

	exec("/usr/sbin/drbdsetup show",$sl,$rt);
	foreach($sl as $n => $d)
	{
		$d=preg_replace("/\#.*$/","",$d);
		$d=ltrim(rtrim($d));
		$sl[$n]=preg_replace("/^(\S+)\s+/","$1:",$d);
	}
	for($lpos=0;$lpos<count($sl);$lpos++)
		if (preg_match("/^(\S+):(\S*) *\{$/",$sl[$lpos],$m))
		{
			if ($m[1] == "resource")
			{
				$res[$m[2]]=parsearray("resource",$sl);
			}
		}
	$sl=array();
	exec("/usr/bin/drbdstats -A -d\|",$sl,$rt);
	if($rt>=0)
	{
		foreach($sl as $l)
		{
			$pts=explode('|',$l);
			$pt=array();
			foreach($pts as $ll)
			{
				if(strpos($ll,":")!== false)
				{
					$v=explode(':',$ll);
					$pt[$v[0]]=$v[1];
				}
			}
			foreach($pt as $p1 => $p2)
				$res[$pt['res']]['connection'][$pt['conn']]['debug'][$p1]=$p2;
		}
	}
	return ($res);
}

