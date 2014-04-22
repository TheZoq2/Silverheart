
--The names of the parts involved
door1Name = "m_al1_d2"
door2Name = "m_al1_d2"

if(m_al_state == 0) then
	m_al_state = 1

	movePartToPart(door1Name, "m_al1_d1_t1");
	movePartToPart(door2Name, "m_al1_d2_t2");
else
	m_al_state = 0
	printToConsole("Airlock state = 0")

	movePartToPart(door1Name, door2Name);
	movePartToPart(door2Name, door1Name);
end