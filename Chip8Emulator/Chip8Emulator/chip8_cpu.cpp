#include "chip8_cpu.h"

Chip8Cpu::Chip8Cpu()
	: program_counter_(0)
	, index_register_(0)
	, gp_register_{0}
	, delay_timer_(0)
	, sound_timer_(0)
	, memory_()
	, opcode_(0)
	, beep_(kBeepFilename)
{
	LoadFont();
	rng_.seed(kSeed);
}

void Chip8Cpu::Start(std::string filename)
{
	LoadRom(filename);
	this->program_counter_ = kRomAddress;

	while (true)
	{
		HandleTimers();

		Cycle();

		// TODO this is called everytime to re-draw last issued data. Pero el motivo es para ejecutar el polliwg de los eventos. No me gusta, lo tendre que cambiar.
		this->display_.Render();

		//std::this_thread::sleep_for(std::chrono::milliseconds(10));
		//std::this_thread::sleep_for(std::chrono::microseconds(1000));
		std::this_thread::sleep_for(std::chrono::nanoseconds(400));
	}
}

void Chip8Cpu::LoadRom(std::string filename)
{
	Chip8Rom rom_(filename);
	for (unsigned short i = 0; i < rom_.size_; i++)
	{
		this->memory_.Write(kRomAddress + i, rom_.content_[i]);
	}
}

void Chip8Cpu::LoadFont()
{
	for (unsigned char i = 0; i < sizeof(kFont); i++)
	{
		this->memory_.Write(kFontAddress + i, kFont[i]);
	}
}

void Chip8Cpu::Cycle()
{
	Fetch();
	Decode();
	Execute();
}

// TODO: Control not to go out of memory on fetching (i.e. try to fetch an opcode from a non-valid address)
void Chip8Cpu::Fetch()
{
	unsigned char high_byte = this->memory_.Read(this->program_counter_);
	unsigned char low_byte = this->memory_.Read(this->program_counter_ + (unsigned short)1);

	this->opcode_ = high_byte << 8 | low_byte;
	LogFetchedOpcode();

	NextInstruction();
}

void Chip8Cpu::NextInstruction()
{
	this->program_counter_ += 2;
}

void Chip8Cpu::PreviousInstruction()
{
	this->program_counter_ -= 2;
}

// TODO: Throw (and handle) error in case decoding fails; refactor (get rid of nested switch statements) so do not to repeat error handle in each switch statement
void Chip8Cpu::Decode()
{
	switch (this->opcode_ & 0xF000)
	{
		case 0x0000:
		{
			// 0XXX instruction family
			switch (this->opcode_)
			{
				case 0x00E0:
				{
					LogDecodedInstruction("00E0");
					this->instruction_ = Instruction::I00E0;
					break;
				}
				case 0x00EE:
				{
					LogDecodedInstruction("00EE");
					this->instruction_ = Instruction::I00EE;
					break;
				}
				default:
				{
					LogDecodedInstruction("0NNN - Warning! Not supported");
					this->instruction_ = Instruction::I0NNN;
					break;
				}
			}
			break;
		}
		case 0x1000:
		{
			LogDecodedInstruction("1NNN");
			this->instruction_ = Instruction::I1NNN;
			break;
		}
		case 0x2000:
		{
			LogDecodedInstruction("2NNN");
			this->instruction_ = Instruction::I2NNN;
			break;
		}
		case 0x3000:
		{
			LogDecodedInstruction("3XNN");
			this->instruction_ = Instruction::I3XNN;
			break;
		}
		case 0x4000:
		{
			LogDecodedInstruction("4XNN");
			this->instruction_ = Instruction::I4XNN;
			break;
		}
		case 0x5000:
		{
			LogDecodedInstruction("5XY0");
			this->instruction_ = Instruction::I5XY0;
			break;
		}
		case 0x6000:
		{
			LogDecodedInstruction("6XNN");
			this->instruction_ = Instruction::I6XNN;
			break;
		}
		case 0x7000:
		{
			LogDecodedInstruction("7XNN");
			this->instruction_ = Instruction::I7XNN;
			break;
		}
		case 0x8000:
		{
			// 8XXX instruction family
			switch (this->opcode_ & 0x000F)
			{
				case 0x0000:
				{
					LogDecodedInstruction("8XY0");
					this->instruction_ = Instruction::I8XY0;
					break;
				}
				case 0x0001:
				{
					LogDecodedInstruction("8XY1");
					this->instruction_ = Instruction::I8XY1;
					break;
				}
				case 0x0002:
				{
					LogDecodedInstruction("8XY2");
					this->instruction_ = Instruction::I8XY2;
					break;
				}
				case 0x0003:
				{
					LogDecodedInstruction("8XY3");
					this->instruction_ = Instruction::I8XY3;
					break;
				}
				case 0x0004:
				{
					LogDecodedInstruction("8XY4");
					this->instruction_ = Instruction::I8XY4;
					break;
				}
				case 0x0005:
				{
					LogDecodedInstruction("8XY5");
					this->instruction_ = Instruction::I8XY5;
					break;
				}
				case 0x0006:
				{
					LogDecodedInstruction("8XY6");
					this->instruction_ = Instruction::I8XY6;
					break;
				}
				case 0x0007:
				{
					LogDecodedInstruction("8XY7");
					this->instruction_ = Instruction::I8XY7;
					break;
				}
				case 0x000E:
				{
					LogDecodedInstruction("8XYE");
					this->instruction_ = Instruction::I8XYE;
					break;
				}
				default:
				{
					LogDecodedInstruction("Error! Unknown");
					// TODO: Throw error! And handle it.
					this->instruction_ = 0;
					break;
				}	
			}
			break;
		}
		case 0x9000:
		{
			LogDecodedInstruction("9XY0");
			this->instruction_ = Instruction::I9XY0;
			break;
		}
		case 0xA000:
		{
			LogDecodedInstruction("ANNN");
			this->instruction_ = Instruction::IANNN;
			break;
		}
		case 0xB000:
		{
			LogDecodedInstruction("BNNN");
			this->instruction_ = Instruction::IBNNN;
			break;
		}
		case 0xC000:
		{
			LogDecodedInstruction("CXNN");
			this->instruction_ = Instruction::ICXNN;
			break;
		}
		case 0xD000:
		{
			LogDecodedInstruction("DXYN");
			this->instruction_ = Instruction::IDXYN;
			break;
		}
		case 0xE000:
		{
			// EXXX instruction family
			switch (this->opcode_ & 0x00FF)
			{
				case 0x009E:
				{
					LogDecodedInstruction("EX9E");
					this->instruction_ = Instruction::IEX9E;
					break;
				}
				case 0x00A1:
				{
					LogDecodedInstruction("EXA1");
					this->instruction_ = Instruction::IEXA1;
					break;
				}
				default:
				{
					LogDecodedInstruction("Error! Unknown");
					// TODO: Throw error! And handle it.
					this->instruction_ = 0;
					break;
				}
			}
			break;
		}
		case 0xF000:
		{
			// FXXX instruction family
			switch (this->opcode_ & 0x00FF)
			{
				case 0x0007:
				{
					LogDecodedInstruction("FX07");
					this->instruction_ = Instruction::IFX07;
					break;
				}
				case 0x000A:
				{
					LogDecodedInstruction("FX0A");
					this->instruction_ = Instruction::IFX0A;
					break;
				}
				case 0x0015:
				{
					LogDecodedInstruction("FX15");
					this->instruction_ = Instruction::IFX15;
					break;
				}
				case 0x0018:
				{
					LogDecodedInstruction("FX18");
					this->instruction_ = Instruction::IFX18;
					break;
				}
				case 0x001E:
				{
					LogDecodedInstruction("FX1E");
					this->instruction_ = Instruction::IFX1E;
					break;
				}
				case 0x0029:
				{
					LogDecodedInstruction("FX29");
					this->instruction_ = Instruction::IFX29;
					break;
				}
				case 0x0033:
				{
					LogDecodedInstruction("FX33");
					this->instruction_ = Instruction::IFX33;
					break;
				}
				case 0x0055:
				{
					LogDecodedInstruction("FX55");
					this->instruction_ = Instruction::IFX55;
					break;
				}
				case 0x0065:
				{
					LogDecodedInstruction("FX65");
					this->instruction_ = Instruction::IFX65;
					break;
				}
				default:
				{
					LogDecodedInstruction("Error! Unknown");
					// TODO: Throw error! And handle it.
					this->instruction_ = 0;
					break;
				}
			}
			break;
		}
		default:
		{
			LogDecodedInstruction("Error! Unknown");
			// TODO: Throw error! And handle it.
			this->instruction_ = 0;
			break;
		}
	}
}

// TODO: Ensure an instruction was actually decoded before try executing anything.
void Chip8Cpu::Execute()
{
	switch (this->instruction_)
	{
		case Instruction::I0NNN:
		{
			// Warning! This instruction could be only implemented if emulating the whole microcomputer (it would be executing corresponding processor instructions).
			break;
		}
		case Instruction::I00E0:
		{
			ClearDisplay();
			break;
		}
		case Instruction::I00EE:
		{
			unsigned short address = this->memory_.Pop();
			this->program_counter_ = address;
			break;
		}
		case Instruction::I1NNN:
		{
			unsigned short address = DecodeNNN();
			this->program_counter_ = address;
			break;
		}
		case Instruction::I2NNN:
		{
			unsigned short address = DecodeNNN();
			this->memory_.Push(this->program_counter_);
			this->program_counter_ = address;
			break;
		}
		case Instruction::I3XNN:
		{
			unsigned char gp_register_index = DecodeX();
			unsigned char value = DecodeNN();
			if (this->gp_register_[gp_register_index] == value)
			{
				NextInstruction();
			}
			break;
		}
		case Instruction::I4XNN:
		{
			unsigned char gp_register_index = DecodeX();
			unsigned char value = DecodeNN();
			if (this->gp_register_[gp_register_index] != value)
			{
				NextInstruction();
			}
			break;
		}
		case Instruction::I5XY0:
		{
			unsigned char gp_register_index_x = DecodeX();
			unsigned char gp_register_index_y = DecodeY();
			if (this->gp_register_[gp_register_index_x] == this->gp_register_[gp_register_index_y])
			{
				NextInstruction();
			}
			break;
		}
		case Instruction::I6XNN:
		{
			unsigned char gp_register_index = DecodeX();
			unsigned char value = DecodeNN();
			this->gp_register_[gp_register_index] = value;
			break;
		}
		case Instruction::I7XNN:
		{
			unsigned char gp_register_index = DecodeX();
			unsigned char value = DecodeNN();
			this->gp_register_[gp_register_index] += value;
			break;
		}
		case Instruction::I8XY0:
		{
			unsigned char gp_register_index_x = DecodeX();
			unsigned char gp_register_index_y = DecodeY();
			this->gp_register_[gp_register_index_x] = this->gp_register_[gp_register_index_y];
			break;
		}
		case Instruction::I8XY1:
		{
			unsigned char gp_register_index_x = DecodeX();
			unsigned char gp_register_index_y = DecodeY();
			this->gp_register_[gp_register_index_x] |= this->gp_register_[gp_register_index_y];
			break;
		}
		case Instruction::I8XY2:
		{
			unsigned char gp_register_index_x = DecodeX();
			unsigned char gp_register_index_y = DecodeY();
			this->gp_register_[gp_register_index_x] &= this->gp_register_[gp_register_index_y];
			break;
		}
		case Instruction::I8XY3:
		{
			unsigned char gp_register_index_x = DecodeX();
			unsigned char gp_register_index_y = DecodeY();
			this->gp_register_[gp_register_index_x] ^= this->gp_register_[gp_register_index_y];
			break;
		}
		case Instruction::I8XY4:
		{
			unsigned char gp_register_index_x = DecodeX();
			unsigned char gp_register_index_y = DecodeY();
			unsigned short addition = this->gp_register_[gp_register_index_x] + this->gp_register_[gp_register_index_y];
			this->gp_register_[gp_register_index_x] = addition & 0xFF;  // Get 8 LSB bits (1 B) as addition result
			// Warning! Do not need to worry about 8 MSB bits (15-8 bit from 2 B) on result checking because greatest possible value is a 9 bit number (0xFF + 0xFF == 0x1FE == 0b1 1111 1110)
			this->gp_register_[0xF] = addition >> 0x8;  // Set flag register VF (general purpose register 0xF) on overflow
			break;
		}
		case Instruction::I8XY5:
		{
			unsigned char gp_register_index_x = DecodeX();
			unsigned char gp_register_index_y = DecodeY();
			unsigned short substraction = this->gp_register_[gp_register_index_x] - this->gp_register_[gp_register_index_y];
			this->gp_register_[gp_register_index_x] = substraction & 0xFF;  // Get 8 LSB bits (1 B) as substraction result
			this->gp_register_[0xF] = !(substraction >> 0x8);  // Unset flag register VF (general purpose register 0xF) on underflow
			break;
		}
		case Instruction::I8XY6:
		{
			// TODO: implement variation (VX=VY in the very beginning)
			unsigned char gp_register_index_x = DecodeX();
			this->gp_register_[0xF] = this->gp_register_[gp_register_index_x] & 0x01;  // Get LSB (1st bit)
			this->gp_register_[gp_register_index_x] = this->gp_register_[gp_register_index_x] >> 1;
			break;
		}
		case Instruction::I8XY7:
		{
			unsigned char gp_register_index_x = DecodeX();
			unsigned char gp_register_index_y = DecodeY();
			unsigned short substraction = this->gp_register_[gp_register_index_y] - this->gp_register_[gp_register_index_x];
			this->gp_register_[gp_register_index_x] = substraction & 0xFF;  // Get 8 LSB bits (1 B) as substraction result
			this->gp_register_[0xF] = !(substraction >> 0x8);  // Unset flag register VF (general purpose register 0xF) on underflow
			break;
		}
		case Instruction::I8XYE:
		{
			// TODO: implement variation (VX=VY in the very beginning)
			unsigned char gp_register_index_x = DecodeX();
			this->gp_register_[0xF] = this->gp_register_[gp_register_index_x] & 0x80 ? 0x1 : 0x0;  // Get MSB (8th bit)
			this->gp_register_[gp_register_index_x] = this->gp_register_[gp_register_index_x] << 1;
			break;
		}
		case Instruction::I9XY0:
		{
			unsigned char gp_register_index_x = DecodeX();
			unsigned char gp_register_index_y = DecodeY();
			if (this->gp_register_[gp_register_index_x] != this->gp_register_[gp_register_index_y])
			{
				NextInstruction();
			}
			break;
		}
		case Instruction::IANNN:
		{
			unsigned short address = DecodeNNN();
			this->index_register_ = address;
			break;
		}
		case Instruction::IBNNN:
		{
			unsigned short address = DecodeNNN();
			unsigned char value = this->gp_register_[0];
			this->program_counter_ = address + value;
			break;
		}
		case Instruction::ICXNN:
		{
			unsigned char gp_register_index_x = DecodeX();
			unsigned char value = DecodeNN();
			std::uniform_int_distribution<uint32_t> uint_dist256(0x0, 0xFF);  // TODO: relocate (this should inly be generated once)
			unsigned char randomNumber = uint_dist256(rng_); // 8 bit random number
			this->gp_register_[gp_register_index_x] = randomNumber & value;
			break;
		}
		case Instruction::IDXYN:
		{
			unsigned char gp_register_index_x = DecodeX();
			unsigned char gp_register_index_y = DecodeY();
			unsigned char sprite_height = DecodeN();
			DrawSprite(this->gp_register_[gp_register_index_x], this->gp_register_[gp_register_index_y], sprite_height);
			break;
		}
		case Instruction::IEX9E:
		{
			unsigned char gp_register_index = DecodeX();
			unsigned char value = this->gp_register_[gp_register_index];			
			if (this->display_.IsKeyPressed(value))
			{
				NextInstruction();
			}
			break;
		}
		case Instruction::IEXA1:
		{
			unsigned char gp_register_index = DecodeX();
			unsigned char value = this->gp_register_[gp_register_index];
			if (!this->display_.IsKeyPressed(value))
			{
				NextInstruction();
			}
			break;
		}
		case Instruction::IFX07:
		{
			unsigned char gp_register_index_x = DecodeX();
			this->gp_register_[gp_register_index_x] = this->delay_timer_;
			break;
		}
		case Instruction::IFX0A:
		{
			unsigned char value;
			if (this->display_.GetKeyPressed(&value)) {
				unsigned char gp_register_index = DecodeX();
				this->gp_register_[gp_register_index] = value;
			}
			else
			{
				PreviousInstruction();
			}
			break;
		}
		case Instruction::IFX15:
		{
			unsigned char gp_register_index_x = DecodeX();
			this->delay_timer_ = this->gp_register_[gp_register_index_x];
			break;
		}
		case Instruction::IFX18:
		{
			unsigned char gp_register_index_x = DecodeX();

			if (!this->sound_timer_)
			{
				// Play (async) sound (in loop)
				this->beep_.Play();
			}

			this->sound_timer_ = this->gp_register_[gp_register_index_x];
			break;
		}
		case Instruction::IFX1E:
		{
			// TODO: implement variation (set VF on 12-bit overflow)
			unsigned char gp_register_index_x = DecodeX();
			unsigned char value = this->gp_register_[gp_register_index_x];
			this->index_register_ += value;
			break;
		}
		case Instruction::IFX29:
		{
			unsigned char gp_register_index_x = DecodeX();
			unsigned char value = this->gp_register_[gp_register_index_x];
			this->index_register_ = kFontAddress + (value * 5); // Each font sprite is made of 5 B
			break;
		}
		case Instruction::IFX33:
		{
			unsigned char gp_register_index_x = DecodeX();
			unsigned char value = this->gp_register_[gp_register_index_x];
			// TODO: implement BCD calculation in separated function.
			this->memory_.Write(this->index_register_, (value / 100) % 10);
			this->memory_.Write(this->index_register_ + 1, (value / 10) % 10);
			this->memory_.Write(this->index_register_ + 2, value % 10);
			break;
		}
		case Instruction::IFX55:
		{
			// TODO: implement variation (update I after each calculation)
			unsigned char gp_register_index_x = DecodeX();
			for (unsigned char i = 0; i <= gp_register_index_x; i++)
			{
				this->memory_.Write(this->index_register_ + i, this->gp_register_[i]);
			}
			break;
		}
		case Instruction::IFX65:
		{
			// TODO: implement variation (update I after each calculation)
			unsigned char gp_register_index_x = DecodeX();
			for (unsigned char i = 0; i <= gp_register_index_x; i++)
			{
				this->gp_register_[i] = this->memory_.Read(this->index_register_ + i);
			}
			break;
		}
		default:
			break;
	}
}

unsigned char Chip8Cpu::DecodeX()
{
	return (this->opcode_ & (unsigned short)0x0F00) >> 8;
}

unsigned char Chip8Cpu::DecodeY()
{
	return (this->opcode_ & (unsigned short)0x00F0) >> 4;
}

unsigned char Chip8Cpu::DecodeN()
{
	return this->opcode_ & (unsigned short)0x000F;
}

unsigned char Chip8Cpu::DecodeNN()
{
	return this->opcode_ & (unsigned short)0x00FF;
}

unsigned short Chip8Cpu::DecodeNNN()
{
	return this->opcode_ & (unsigned short)0x0FFF;
}

void Chip8Cpu::ClearDisplay()
{
	unsigned char* p = this->memory_.GetFramebuffer();
	for (unsigned short i = 0; i < 64*32; i++)
	{
		p[i] = 0x00;
	}

	this->display_.Render(p);
}

void Chip8Cpu::DrawSprite(unsigned char at_x, unsigned char at_y, unsigned char sprite_height)
{
	// Reset collision flag
	this->gp_register_[0xF] = 0x00;

	// Wrap display coordinate
	at_x %= 64;
	at_y %= 32;

	unsigned char* p = this->memory_.GetFramebuffer();

	for (unsigned char i = 0; i < sprite_height; i++)
	{
		// Iterate by sprite level - i.e. byte - from bottom to top
		unsigned char sprite_level = this->memory_.Read(this->index_register_ + i);

		// Determinar cual es el indice del array de la memoria que corresponde a la coordenada "at" teniendo en cuenta:
		// - el nivel de sprite en curso (iterador "i)
		// - el bit dentro del byte (nivel de sprite; iterador "j")
		// Warning! En una implementacion si nos salimos de coordenadas, entonces no se calcula nada; en la otra, se hace un wraping en el eje correspondiente.

		unsigned char offset_at_y = at_y + i;
		if (!(offset_at_y < 32)) {
			// TODO: En el modo alternativo (en que tambien se aplica wraping al dibujado), aqui bastaria con aplicar el modulo 64. En este caso terminamos la ejecion del bucle.
			break;  // offset_at_y %= 64;
		}

		for (unsigned char j = 0; j < 8; j++)
		{
			// Iterate by level bit
			unsigned char offset_at_x = at_x + j;
			if (!(offset_at_x < 64)) {
				// TODO: Lo mismo de arriba sobre el modo alternativo
				break;  // offset_at_x %= 32;
			}

			if (!(sprite_level & (0b10000000 >> j)))
			{
				// No es necesario hacer ninguna comprobacion ni cambios si es que el nuevo pixel a escribir es un 0
				continue;
			}

			// TODO: no tengo claro si la coordenada at esta zero-based o no. En caso de que NO lo fuera tendriamos que restar 1 a cada componente al hacer esta operacion.
			// TODO: Ojo! A la hora de calcular la posicion en el array invertimos el eje Y, ya que el sentido del eje +Y en los sitema de coordenadas del display chip8 y de la textura 2d de opengl son opuestos.
			// Esta inversion se podria haber hecho despues en el Render, pero seria algo mas complicado (realmente tendria que darle una vuelta para saber de quien deberia ser realmente la responsabilidad de esta tarea)
			// Tambien la podriamos haber hecho antes a la hora de definier la variable offset_at_y, pero me interesa dejar eso sin tocar - limpio- y hacer el cambio solo aqui a la hora de hacer el calculo de la posicion en el array de la memorya del display.
			unsigned short framebuffer_index = 64 * (31 - offset_at_y) + offset_at_x;
			this->gp_register_[0xF] |= p[framebuffer_index] ? 0x1 : 0x0;  // Llegados a este punto, vamos a tener que modificar el pixel correspondiente. Si en cache hay algo, indicar colision (al usar el OR, si ya se hubiera puesta el flag antes no se borraria en los casos en que bo hubiera)
			
			// Me huele que esto puede no estar comportandose siempre como espero. pruebo a hacerlo hardcoded.
			// Effectivamente, el problema esta aqui, pero no entiendo por que??? Revisarlo.
			// ??FIN! EL problema era el puto operador NOT !, que lo que hacia era devolverme un 0x01 en lugar de 0xFF. Ahora con el operador ~ ya funciona.
			p[framebuffer_index] = ~p[framebuffer_index];
		}		
	}

	this->display_.Render(p);
}

void Chip8Cpu::HandleTimers()
{
	if (this->delay_timer_)
	{
		this->delay_timer_--;
	}

	if (this->sound_timer_)
	{
		this->sound_timer_--;

		if (!this->sound_timer_)
		{
			this->beep_.Stop();
		}
	}
}

void Chip8Cpu::LogFetchedOpcode()
{
	std::cout << "($";
	std::cout << std::hex << this->program_counter_;
	std::cout << ") Opcode: ";
	std::cout << std::hex << this->opcode_;
	std::cout << " - ";
}

void Chip8Cpu::LogDecodedInstruction(std::string instruction)
{
	std::cout << "Instruction: " + instruction;
	std::cout << "\n";
}
