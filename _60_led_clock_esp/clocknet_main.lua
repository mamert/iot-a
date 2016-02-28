


function init_i2c(sda,scl)
	i2c.setup(0, sda, scl, i2c.SLOW)
end

function send_to_duino(cmd, data)
	local duino = 0x40 -- chosen arbitrarily
	i2c.start(0)
	i2c.address(0, duino ,i2c.TRANSMITTER)
	-- print("Sent "..i2c.write(0, data))
	-- print("Sent "..i2c.write(0, ""..cmd..data))
	print("Sent "..i2c.write(0, cmd, data))
	i2c.stop(0)
	-- use timer6, delay read so arduino can finish
	tmr.alarm(6, 150, 0, function() 
		i2c.start(0)
		i2c.address(0, duino,i2c.RECEIVER)
		ret=i2c.read(0,20) -- response is 20 bytes
		i2c.stop(0)
		print("Received "..ret)
    end )
end






function contactNTP()
	local timeserver = "3.pool.ntp.org"
	--TODO: on fail cycle servers
	-- net.dns.resolve("3.pool.ntp.org", function (dontUseThisSocket, ip)
				 -- if ip == nil then print("3.pool.ntp.org DNS fail!") else print(ip) end
	-- end )
	sntp.sync(timeserver,
	  function(sec,usec,server)
		print("sync", sec, usec, server)
		send_to_duino('T', ""..(sec+1))
	  end,
	  function()
	   print("sync failed!")
	  end
	)
end


button1Pin = 3 -- "flash" btn
function pressed1()
	tmr.delay(10)                   
	gpio.trig(button1Pin,"up",released1)
end
function released1()
	tmr.delay(10)
	gpio.trig(button1Pin,"down",pressed1)
	contactNTP()
end
function prep_button1()
	gpio.mode(button1Pin,gpio.INT,gpio.PULLUP)
	gpio.trig(button1Pin,"down",pressed1)
end


-- program
init_i2c(6,7)
prep_button1()
