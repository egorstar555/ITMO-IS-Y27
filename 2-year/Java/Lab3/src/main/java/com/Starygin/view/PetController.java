package com.Starygin.view;

import com.Starygin.model.entity.Pet;
import com.Starygin.model.dto.PetDto;
import com.Starygin.service.PetService;
import com.Starygin.service.exceptions.PetIdMismatchException;
import com.querydsl.core.types.Predicate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Pageable;
import org.springframework.data.querydsl.binding.QuerydslPredicate;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Objects;

@RestController
@RequestMapping("/api/pets")
public class PetController {

    private final PetService petService;

    @Autowired
    public PetController(PetService petService) {
        this.petService = petService;
    }

    @GetMapping
    public List<PetDto> findAll() {
        return petService.findAll();
    }

    @GetMapping("/page-and-filter")
    public List<PetDto> findAllByPageAndFilter(
            @QuerydslPredicate(root = Pet.class) Predicate predicate,
            Pageable pageable) {

        return petService.findAllByPageAndFilter(predicate, pageable);
    }

    @GetMapping("/name/{petName}")
    public List<PetDto> findByName(@PathVariable String petName) {
        return petService.findByName(petName);
    }

    @GetMapping("/{id}")
    public PetDto findOne(@PathVariable Long id) {
        return petService.findOne(id);
    }

    @PostMapping
    @ResponseStatus(HttpStatus.CREATED)
    public PetDto create(@RequestBody PetDto petDto) {
        return petService.create(petDto);
    }

    @DeleteMapping("/{id}")
    public void delete(@PathVariable Long id) {
        petService.delete(id);
    }

    @PutMapping("/{id}")
    public PetDto updatePet(@RequestBody PetDto petDto, @PathVariable Long id) {
        if (!Objects.equals(petDto.id(), id)) {
            throw new PetIdMismatchException();
        }

        return petService.updatePet(petDto, id);
    }
}
