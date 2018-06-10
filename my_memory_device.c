#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <asm/uaccess.h> /* copy_from/to_user */
#include <linux/sort.h> /* sort */

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Jeova Pereira Gomes");

#define SUCCESS 0
#define DEVICE_NAME "memory"
#define BUFFER_LENGTH 10

/* Declaração das funções memory.c  */
static int memory_open(struct inode *inode, struct file *filp);
static int memory_release(struct inode *inode, struct file *filp);
static ssize_t memory_read(struct file *filp,  char *buf, size_t count, loff_t *f_pos);
static ssize_t memory_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
void memory_exit(void);
int memory_init(void);

static int compare(const void *a, const void *b); /* Funcao para comparar no sort */


static struct file_operations memory_fops = {
  .owner = THIS_MODULE,
  .read = memory_read,
  .write = memory_write,
  .open = memory_open,
  .release = memory_release
};

/* Declaração das funções init and exit */
module_init(memory_init);
module_exit(memory_exit);

/* GLOBAL */
static char msg[BUFFER_LENGTH];
static char *msg_p;
int memory_major = 0;
int bytes_operated = 0;

int memory_init(void) {
  /*
  	Registrando o dispositivo 
	0, para alocação dinamica
   */
  memory_major = register_chrdev(0, DEVICE_NAME, &memory_fops);
  
  // Falha
  if (memory_major < 0) {
    printk(KERN_ALERT "[MEMORY]: Major number nao pode ser obtido %d\n", memory_major);
    return memory_major;
  }else
    printk(KERN_ALERT "[MEMORY] Registrado com o Major number: %d\n", memory_major);
  
  return SUCCESS;
}

void memory_exit(void) {
  /* Liberando o major number */
  unregister_chrdev(memory_major, DEVICE_NAME); 
  printk(KERN_ALERT "[MEMORY]: Removendo o modulo\n");
}

//Executado quando o disposotivo é aberto.
static int memory_open(struct inode *inode, struct file *filp) {
 	printk(KERN_DEBUG "[MEMORY]: Aberto");
	msg_p = msg;
   	return SUCCESS;
}

//Executado quando o disposotivo é Fechado
static int memory_release(struct inode *inode, struct file *filp) {
 	printk(KERN_DEBUG "[MEMORY]: Fechado");
    return SUCCESS;
}

/* Parametros: 
     filp - referência para o arquivo de device
	 buf - local onde os dados lidos serão colocados
	 count - quantidade de bytes lidos
	 f_pos - Posição inicial de leitura
	 Retorna o número de bytes lidos com sucesso.
 */
static ssize_t memory_read(struct file *filp,  char *buf, size_t count, loff_t *f_pos) { 
 	printk(KERN_DEBUG "[MEMORY]: Executou o read %s\n\n", msg_p);

	if(*msg_p == '\0') return 0;
	bytes_operated = 0;
	while(count && *msg_p){
		// put_user — Escreve um valor no espaco do usuario
		// put_user(valor, endereco)
		put_user(*(msg_p++), buf++);
		count--;
		bytes_operated++;
	}
	
	return bytes_operated;
}

static ssize_t memory_write( struct file *filp, const char *buf,size_t count, loff_t *posicao) {	
	printk(KERN_DEBUG "[MEMORY]: Executou o write");

	bytes_operated = 0;
	
	// Copia um bloco de dados do espaco do usuario.
	// Retorna o numero de bytes que nao foi copiado
	// Se 0, foi sucesso
	memset(msg_p, 0, sizeof(char)* BUFFER_LENGTH);
	if(copy_from_user(msg_p, buf, count)){
		return -EINVAL;
	}else{
		// Utilizando o sort, para ordenar
		// sort(base, quantidade, tamanho, compare, swap)
		sort(msg_p, count, sizeof(char), &compare, NULL);
		
		printk(KERN_DEBUG "[MEMORY3]: ordenacao,  msg: %s\n", msg_p);	
		return count;
	}
}

// Funcao auxiliar para utilizar no sort
static int compare(const void *a, const void *b){
	char ca = *(const char*)(a);
	char cb = *(const char*)(b);
	if(ca < cb) return -1;
	if(ca > cb) return 1;
	return 0;
}
