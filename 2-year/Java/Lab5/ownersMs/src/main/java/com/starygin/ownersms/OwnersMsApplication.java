package com.starygin.ownersms;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.autoconfigure.domain.EntityScan;
import org.springframework.data.jpa.repository.config.EnableJpaRepositories;

@EnableJpaRepositories("com.starygin.ownersms.repository")
@EntityScan("com.starygin.ownersms.entity")
@SpringBootApplication
public class OwnersMsApplication {

	public static void main(String[] args) {
		SpringApplication.run(OwnersMsApplication.class, args);
	}
}
