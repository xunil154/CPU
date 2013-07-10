#!/usr/bin/perl
use strict;
use warnings;

use Device::SerialPort;
use IPC::Open2;

my $DEV='/dev/ttyACM0';
my $RATE = 9600;

my $TOP_CMD = 'top -b -d 1.5';

while ( ! -e $DEV ){
	print "$DEV does not exist\n";
	$DEV = '/dev/ttyACM1';
	if ( ! -e $DEV ){
		print "Could not find $DEV\n";
	}else{
		last;
	}

	$DEV = '/dev/ttyACM0';
	sleep 2;
}

print "Using $DEV\n";

my $port = Device::SerialPort->new($DEV);
$port->baudrate($RATE);


my $str;
my $state = 0;

my $cpu = 0;

my $used = 0;
my $avail = 0;

#print "Entering main loop\n";
while(1){
	open(TOP_IN,"-|", $TOP_CMD);
#TOP_IN->autoflush(1);
#TOP->autoflush(1);

	select TOP_IN;
	$| = 1;
	select STDOUT;

	my $count = 0;
	my @cpu;
	while (<TOP_IN>){
#    print "=======\n$_\n";

		if($state == 0){
			@cpu = ();
		}


		if($_ =~ m/Cpu(\d+)/i){
			$state = 1;
			my @data = ();
			while($_ =~ /([\d.]+)/g){
				push(@data,$1);
			}
			push(@cpu,$data[0]+$data[1]+$data[2]);
		}

		if($_ =~ /Mem[^\d]+(\d+)[^\d]+(\d+)/){
			$avail = $1;
			$used = $2;

			$str = "#C,".scalar(@cpu);

			foreach (@cpu){
				$str .= ",".int($_);
			}

			my $date = `date +'%I:%M %p'`;
			chomp($date);
			$str .= ",M,".int($used/1000).",".int($avail/1000).",D,$date#";

			print $str."\n";
			$state = 1;
			$port->write($str);
			$state = 0;
		}
	}
}
