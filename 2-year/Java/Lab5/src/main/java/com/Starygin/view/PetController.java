package com.Starygin.view;

import com.Starygin.service.PetServiceKafka;
import com.Starygin.service.dto.PetDto;
import com.Starygin.service.exceptions.PetIdMismatchException;
import com.fasterxml.jackson.core.JsonProcessingException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.security.access.prepost.PreAuthorize;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Objects;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.TimeoutException;

@RestController
@RequestMapping("/api/pets")
public class PetController {

    private final PetServiceKafka petServiceKafka;

    @Autowired
    public PetController(PetServiceKafka petServiceKafka) {
        this.petServiceKafka = petServiceKafka;
    }

    @GetMapping
    public List<PetDto> findAll() throws ExecutionException, InterruptedException, JsonProcessingException, TimeoutException {
        return petServiceKafka.findAll();
    }

    @GetMapping("/name/{petName}")
    public List<PetDto> findByName(@PathVariable String petName) throws ExecutionException, InterruptedException, JsonProcessingException, TimeoutException {
        return petServiceKafka.findByName(petName);
    }

    @GetMapping("/{id}")
    public PetDto findOne(@PathVariable Long id) throws ExecutionException, InterruptedException, JsonProcessingException, TimeoutException {
        return petServiceKafka.findOne(id);
    }

    @PostMapping
    @ResponseStatus(HttpStatus.CREATED)
    @PreAuthorize("hasPermission(#petDto.ownerId(),'owner', 'admin')")
    public PetDto create(@RequestBody PetDto petDto) throws ExecutionException, InterruptedException, JsonProcessingException, TimeoutException {
        return petServiceKafka.create(petDto);
    }

    @DeleteMapping("/{id}")
    @PreAuthorize("hasPermission(#id, 'pet', 'admin')")
    public void delete(@PathVariable Long id) throws ExecutionException, InterruptedException, JsonProcessingException, TimeoutException {
        petServiceKafka.delete(id);
    }

    @PutMapping("/{id}")
    @PreAuthorize("hasPermission(#id, 'pet', 'admin')")
    public PetDto updatePet(@RequestBody PetDto petDto, @PathVariable Long id) throws ExecutionException, InterruptedException, JsonProcessingException, TimeoutException {
        if (!Objects.equals(petDto.id(), id)) {
            throw new PetIdMismatchException();
        }

        return petServiceKafka.updatePet(petDto);
    }
}
